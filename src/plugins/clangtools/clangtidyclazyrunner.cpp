/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of Qt Creator.
**
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 as published by the Free Software
** Foundation with exceptions as appearing in the file LICENSE.GPL3-EXCEPT
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
****************************************************************************/

#include "clangtidyclazyrunner.h"

#include <cpptools/cppcodemodelsettings.h>
#include <cpptools/cpptoolsreuse.h>

#include <utils/synchronousprocess.h>

#include <QDebug>
#include <QDir>
#include <QLoggingCategory>

static Q_LOGGING_CATEGORY(LOG, "qtc.clangtools.runner")

namespace ClangTools {
namespace Internal {

ClangTidyClazyRunner::ClangTidyClazyRunner(const QString &clangExecutable,
                                           const QString &clangLogFileDir,
                                           const Utils::Environment &environment,
                                           QObject *parent)
    : ClangToolRunner(clangExecutable,
                      clangLogFileDir,
                      environment,
                      tr("Clang-Tidy and Clazy"),
                      parent)
{
}

static void addXclangArg(QStringList &arguments,
                         const QString &argName,
                         const QString &argValue = QString())
{
    arguments << QString("-Xclang")
              << argName;
    if (!argValue.isEmpty()) {
        arguments << QString("-Xclang")
                  << argValue;
    }
}

QStringList ClangTidyClazyRunner::constructCommandLineArguments(const QStringList &options)
{
    using namespace CppTools;
    QStringList arguments;

    if (LOG().isDebugEnabled())
        arguments << QString("-v");

    arguments << QString("-fsyntax-only")
              << QString("-serialize-diagnostics")
              << QString(m_logFile);

    const ClangDiagnosticConfig config = CppTools::codeModelSettings()->clangDiagnosticConfig();

    const ClangDiagnosticConfig::TidyMode tidyMode = config.clangTidyMode();
    if (tidyMode != ClangDiagnosticConfig::TidyMode::Disabled) {
        addXclangArg(arguments, QString("-add-plugin"), QString("clang-tidy"));
        if (tidyMode != ClangDiagnosticConfig::TidyMode::File) {
            const QString tidyChecks = config.clangTidyChecks();
            addXclangArg(arguments, QString("-plugin-arg-clang-tidy"), "-checks=" + tidyChecks);
        }
    }

    const QString clazyChecks = config.clazyChecks();
    if (!clazyChecks.isEmpty()) {
        addXclangArg(arguments, QString("-add-plugin"), QString("clang-lazy"));
        addXclangArg(arguments, QString("-plugin-arg-clang-lazy"), config.clazyChecks());
    }

    arguments += options;
    arguments << QDir::toNativeSeparators(filePath());
    return arguments;
}

} // namespace Internal
} // namespace ClangTools