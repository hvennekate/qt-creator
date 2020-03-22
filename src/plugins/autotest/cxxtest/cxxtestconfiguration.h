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

#ifndef CXXTESTCONFIGURATION_H
#define CXXTESTCONFIGURATION_H

#include "../testconfiguration.h"

namespace Autotest {
	namespace Internal {

		class CxxTestConfiguration : public DebuggableTestConfiguration
		{
		public:
			explicit CxxTestConfiguration(const QString &test =  QString(),
						      const QString &method = QString());

			TestOutputReader *outputReader(const QFutureInterface<TestResultPtr> &fi, QProcess *app) const override;
			QStringList argumentsForTestRunner(QStringList *omitted) const override;
			Utils::Environment filteredEnvironment(const Utils::Environment &original) const override;
		private:
			QString test, method;
		};

	} // namespace Internal
} // namespace Autotest

#endif // CXXTESTCONFIGURATION_H
