/****************************************************************************
**
** Copyright (C) 2020 Hendrik Vennekate
** Contact: Hendrik.Vennekate@posteo.de
**
** This file extends Qt Creator to support a customized version of CxxTest.
** (see https://sourceforge.net/projects/cxxtest/)
**
** GNU General Public License Usage
** This file may be used under the terms of the GNU
** General Public License version 3 as published by the Free Software
** Foundation with exceptions as appearing in the file LICENSE.GPL3-EXCEPT
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
****************************************************************************/

#include "cxxoutputreader.h"
#include "cxxtestconfiguration.h"

namespace Autotest {
	namespace Internal {

		CxxTestConfiguration::CxxTestConfiguration(const QString &test, const QString &method)
			: DebuggableTestConfiguration(), // TODO set mode properly
			  test(test),
			  method(method)
		{}

		TestOutputReader *CxxTestConfiguration::outputReader(const QFutureInterface<TestResultPtr> &fi, QProcess *app) const
		{
			return new CxxOutputReader(fi, app, buildDirectory(), projectFile());
		}

		QStringList CxxTestConfiguration::argumentsForTestRunner(QStringList * /* omitted */) const
		{
			if (testCases().empty()) return QStringList();
			return testCases().first().split(" ");
		}

		Utils::Environment CxxTestConfiguration::filteredEnvironment(const Utils::Environment &original) const
		{
			return original;
		}
	} // namespace Internal
} // namespace Autotest


