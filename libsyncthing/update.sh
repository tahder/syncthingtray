#!/bin/bash
set -e
shopt -s nocasematch
shopt -s nullglob

libsyncthingdir=$(dirname "$0")
syncthingrepodir=$libsyncthingdir/go/src/github.com/syncthing/syncthing
special_release_regex='rc|beta|debian'
latest_tag=

echo '==> Updating Syncthing repo'
git -C "$syncthingrepodir" remote update

echo '==> Getting latest tag/release'
for tag in $(git -C "$syncthingrepodir" tag -l --sort=-version:refname 'v*'); do
    if [[ $tag =~ $special_release_regex ]]; then
        continue
    fi
    if [[ $tag ]]; then
        latest_tag=$tag
        break
    fi
done
if [[ -z $latest_tag ]]; then
    echo 'Unable to find latest release tag'
    exit -1
fi
echo "Latest Syncthing release: $latest_tag"

echo '==> Updating CMakeLists.txt'
sed -i -e "s|^\(set(META_SYNCTHING_VERSION \"\(.*\)\")\).*$|set(META_SYNCTHING_VERSION \"$latest_tag\")|" "$libsyncthingdir/CMakeLists.txt"

echo '==> Rebasing libsyncthing'
if git -C "$syncthingrepodir" rev-parse --verify "libsyncthing-$latest_tag" > /dev/null; then
    echo "Nothing to do: A libsyncthing branch \"libsyncthing-$latest_tag\" for the latest release already exists."
    exit 0
fi
git -C "$syncthingrepodir" checkout libsyncthing-latest
git -C "$syncthingrepodir" rebase "$latest_tag"
git -C "$syncthingrepodir" branch "libsyncthing-$latest_tag"

echo '==> Pushing updated/new libsyncthing branches to GitHub'
git -C "$syncthingrepodir" push -fu martchus libsyncthing-latest:libsyncthing-latest
git -C "$syncthingrepodir" push -u martchus "libsyncthing-$latest_tag:libsyncthing-$latest_tag"
