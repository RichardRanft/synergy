/*
 * Deskflow -- mouse and keyboard sharing utility
 * SPDX-FileCopyrightText: (C) 2025 Chris Rizzitello <sithlord48@gmail.com>
 * SPDX-FileCopyrightText: (C) 2016 - 2025 Symless Ltd.
 * SPDX-License-Identifier: GPL-2.0-only WITH LicenseRef-OpenSSL-Exception
 */

#pragma once

#include <QSettings>

#include <QDir>

#include "common/constants.h"

class Settings : public QObject
{
  Q_OBJECT
public:
#if defined(Q_OS_WIN)
  inline const static auto UserDir = QStringLiteral("%1/AppData/Local/%2").arg(QDir::homePath(), kAppName);
  inline const static auto SystemDir = QStringLiteral("C:/ProgramData/%1").arg(kAppName);
#elif defined(Q_OS_MAC)
  inline const static auto UserDir = QStringLiteral("%1/Library/%2").arg(QDir::homePath(), kAppName);
  inline const static auto SystemDir = QStringLiteral("/Library/%1").arg(kAppName);
#else
  inline const static auto UserDir = QStringLiteral("%1/.config/%2").arg(QDir::homePath(), kAppName);
  inline const static auto SystemDir = QStringLiteral("/etc/%1").arg(kAppName);
#endif
  inline const static auto UserSettingFile = QStringLiteral("%1/%2.conf").arg(UserDir, kAppName);
  inline const static auto SystemSettingFile = QStringLiteral("%1/%2.conf").arg(SystemDir, kAppName);

  struct Client
  {
    inline static const auto Binary = QStringLiteral("client/binary");
    inline static const auto InvertScrollDirection = QStringLiteral("client/invertscolldirection");
    inline static const auto LanguageSync = QStringLiteral("client/languageSync");
    inline static const auto RemoteHost = QStringLiteral("client/remotehost");
  };
  struct Core
  {
    inline static const auto CoreMode = QStringLiteral("core/coreMode");
    inline static const auto ElevateMode = QStringLiteral("core/elevateMode");
    inline static const auto Interface = QStringLiteral("core/interface");
    inline static const auto LastVersion = QStringLiteral("core/lastVersion");
    inline static const auto Port = QStringLiteral("core/port");
    inline static const auto PreventSleep = QStringLiteral("core/preventSleep");
    inline static const auto ProcessMode = QStringLiteral("core/processMode");
    inline static const auto Scope = QStringLiteral("core/loadFromSystemScope");
    inline static const auto ScreenName = QStringLiteral("core/screenName");
    inline static const auto StartedBefore = QStringLiteral("core/startedBefore");
  };
  struct Gui
  {
    inline static const auto Autohide = QStringLiteral("gui/autoHide");
    inline static const auto AutoUpdateCheck = QStringLiteral("gui/enableUpdateCheck");
    inline static const auto CloseReminder = QStringLiteral("gui/closeReminder");
    inline static const auto CloseToTray = QStringLiteral("gui/closeToTray");
    inline static const auto LogExpanded = QStringLiteral("gui/logExpanded");
    inline static const auto SymbolicTrayIcon = QStringLiteral("gui/symbolicTrayIcon");
    inline static const auto WindowGeometry = QStringLiteral("gui/windowGeometry");
  };
  struct Log
  {
    inline static const auto File = QStringLiteral("log/file");
    inline static const auto Level = QStringLiteral("log/level");
    inline static const auto ToFile = QStringLiteral("log/toFile");
  };
  struct Security
  {
    inline static const auto CheckPeers = QStringLiteral("security/checkpeerfingerprints");
    inline static const auto Certificate = QStringLiteral("security/certificate");
    inline static const auto KeySize = QStringLiteral("security/keySize");
    inline static const auto TlsEnabled = QStringLiteral("security/tlsEnabled");
  };
  struct Server
  {
    inline static const auto Binary = QStringLiteral("server/binary");
    inline static const auto ExternalConfig = QStringLiteral("server/externalConfig");
    inline static const auto ExternalConfigFile = QStringLiteral("server/externalConfigFile");
  };

  // Enums types used in settings
  // The use of enum classes is not use for these
  // enum classes are more specific when used with QVariant
  // This leads longer function calls in code
  // and longer more cryptic output in the settings file
  // The using of standard enum will just write ints
  // and we can read / write them as if they were ints
  enum ProcessMode
  {
    Service,
    Desktop
  };
  Q_ENUM(ProcessMode)

  /**
   * @brief The elevate mode tristate determines two behaviors on Windows.
   * The matrix for these two behaviors is as follows:
   *               |    sods   |   elevate  |
   *               |-----------|------------|
   *  kAutomatic   |   true    |   false    |
   *  kAlways      |   false   |   true     |
   *  kNever       |   false   |   false    |
   * The first, --stop-on-desk-switch (sods), is passed through the daemon as a
   * command line argument to the server/client, and determines if it restarts
   * when switching Windows desktops (e.g. when Windows UAC dialog pops up).
   * The second, elevate, is passed as a boolean flag to the daemon over IPC,
   * and determines whether the server/client should be started with elevated privileges.
   */
  enum ElevateMode
  {
    Automatic = 0,
    Always = 1,
    Never = 2
  };
  Q_ENUM(ElevateMode)

  enum CoreMode
  {
    None,
    Client,
    Server
  };
  Q_ENUM(CoreMode)

  static Settings *instance();
  static void setSettingFile(const QString &settingsFile = QString());
  static void setValue(const QString &key = QString(), const QVariant &value = QVariant());
  static QVariant value(const QString &key = QString());
  static void restoreDefaultSettings();
  static QVariant defaultValue(const QString &key);
  static bool isWritable();
  static bool isSystemScope();
  static void setScope(bool systemScope);
  static const QString settingsFile();
  static const QString settingsPath();
  static const QString logLevelText();

signals:
  void scopeChanged(bool isSystemScope);
  void writableChanged(bool canWrite);
  void settingsChanged(const QString key);

private:
  explicit Settings(QObject *parent = nullptr);
  Settings *operator=(Settings &other) = delete;
  Settings(const Settings &other) = delete;
  ~Settings() = default;
  static bool isPortableSettings();
  void cleanSettings();
  void initSettings();

  QSettings *m_settings = nullptr;
  QString m_portableSettingsFile = QStringLiteral("%1.conf").arg(kAppName);

  // clang-format off
  inline static const QStringList m_logLevels = {
     QStringLiteral("INFO")
    , QStringLiteral("DEBUG")
    , QStringLiteral("DEBUG1")
    , QStringLiteral("DEBUG2")
  };

  inline static const QStringList m_validKeys = {
      Client::Binary
    , Client::InvertScrollDirection
    , Client::LanguageSync
    , Client::RemoteHost
    , Core::CoreMode
    , Core::ElevateMode
    , Core::Interface
    , Core::LastVersion
    , Core::Port
    , Core::PreventSleep
    , Core::ProcessMode
    , Core::Scope
    , Core::ScreenName
    , Core::StartedBefore
    , Log::File
    , Log::Level
    , Log::ToFile
    , Gui::Autohide
    , Gui::AutoUpdateCheck
    , Gui::CloseReminder
    , Gui::CloseToTray
    , Gui::LogExpanded
    , Gui::SymbolicTrayIcon
    , Gui::WindowGeometry
    , Security::Certificate
    , Security::CheckPeers
    , Security::KeySize
    , Security::TlsEnabled
    , Server::Binary
    , Server::ExternalConfig
    , Server::ExternalConfigFile
  };
  // clang-format on

#ifdef Q_OS_WIN
  inline static const auto defaultProcessMode = Settings::ProcessMode::Service;
#else
  inline static const auto defaultProcessMode = Settings::ProcessMode::Desktop;
#endif
};
