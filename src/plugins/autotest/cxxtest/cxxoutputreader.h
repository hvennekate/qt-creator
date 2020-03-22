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

#ifndef CXXOUTPUTREADER_H
#define CXXOUTPUTREADER_H

#include "../testoutputreader.h"

#include <QStack>

namespace Autotest {
	namespace Internal {

		class CxxOutputReader : public TestOutputReader
		{
		public:
			CxxOutputReader(const QFutureInterface<TestResultPtr> &futureInterface,
					QProcess *testApplication, const QString &buildDirectory,
					const QString &projectFile);
		protected:
			void processOutputLine(const QByteArray &outputLineWithNewLine) override;
			TestResultPtr createDefaultResult() const override;
		private:
			QStack<const TestResult*> parents;
			const TestResult *currentParent() const;
			QString project;
			QString worldName;
			QString suiteName;
			QString testName;
			static long processCounter;
		};

	} // namespace Internal
} // namespace Autotest

#endif // CXXOUTPUTREADER_H
