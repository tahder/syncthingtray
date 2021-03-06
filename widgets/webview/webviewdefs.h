// Created via CMake from template webviewdefs.h.in
// WARNING! Any changes to this file will be overwritten by the next CMake run!

#ifndef SYNCTHINGWIDGETS_WEB_VIEW_DEFINES
#define SYNCTHINGWIDGETS_WEB_VIEW_DEFINES

#include <QtGlobal>

#if defined(SYNCTHINGWIDGETS_USE_WEBENGINE)
# define SYNCTHINGWIDGETS_WEB_VIEW QWebEngineView
# define SYNCTHINGWIDGETS_WEB_PAGE QWebEnginePage
#elif defined(SYNCTHINGWIDGETS_USE_WEBKIT)
# define SYNCTHINGWIDGETS_WEB_VIEW QWebView
# define SYNCTHINGWIDGETS_WEB_PAGE QWebPage
# define SYNCTHINGWIDGETS_WEB_FRAME QWebFrame
#elif !defined(SYNCTHINGWIDGETS_NO_WEBVIEW)
# error "No definition for web view provider present."
#endif

#ifdef SYNCTHINGWIDGETS_WEB_VIEW
QT_FORWARD_DECLARE_CLASS(SYNCTHINGWIDGETS_WEB_VIEW)
#endif
#ifdef SYNCTHINGWIDGETS_WEB_PAGE
QT_FORWARD_DECLARE_CLASS(SYNCTHINGWIDGETS_WEB_PAGE)
#endif
#ifdef SYNCTHINGWIDGETS_WEB_FRAME
QT_FORWARD_DECLARE_CLASS(SYNCTHINGWIDGETS_WEB_FRAME)
#endif

#endif // SYNCTHINGWIDGETS_WEB_VIEW_DEFINES
