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
#include "cxxtestresult.h"
#include "cxxtesttreeitem.h"
#include <QFileInfo>
#include <QDir>

namespace Autotest {
	namespace Internal {

		long CxxOutputReader::processCounter = 0;

		CxxOutputReader::CxxOutputReader(const QFutureInterface<TestResultPtr> &futureInterface,
						 QProcess *testApplication, const QString &buildDirectory, const QString &projectFile)
			: TestOutputReader(futureInterface, testApplication, buildDirectory),
			  project(projectFile)
		{
			setId(QString::number(processCounter++));
		}

		void CxxOutputReader::processOutputLine(const QByteArray &outputLineWithNewLine)
		{
			static QRegExp start("^Starting (test|suite|world):  (.+)$");
			static QRegExp completion("^(Completed|Skipped|Failed) (test|suite|world): +(\\S+)$");
			static QRegExp debug("^      (Debug|Info|Warning|Critical|Fatal): \\[([^:]+):([0-9]+), [^\\]]+\\] (.+)$");
			static QRegExp worldIntro("^Running .+ tests \\([0-9]+ tests?\\)$");
			static QRegExp worldOutro("^OK!$");
			static QRegExp location("^In ([^:]+::)*[^:]+:$");
			static QRegExp frameworkMessage("^([^:]+):([0-9]+): (Error|Warning): (.+)$"); // TODO use QRegularExpression and named captures
			QString line = removeCommandlineColors(QString::fromUtf8(outputLineWithNewLine));
			if (location.exactMatch(line)) return;
			if (line.isEmpty()) return;

			if (frameworkMessage.exactMatch(line)) {
				QString file = frameworkMessage.cap(1);
				int line = frameworkMessage.cap(2).toInt();
				QString type = frameworkMessage.cap(3);
				QString message = frameworkMessage.cap(4);
				auto result = createDefaultResult();
				result->setFileName(QFileInfo(QDir(m_buildDir), file).canonicalFilePath());
				result->setLine(line);
				result->setResult(ResultType::MessageLocation);
				if ("Error" == type) result->setResult(ResultType::MessageFatal);
				if ("Warning" == type) result->setResult(ResultType::MessageWarn);
				result->setDescription(message);
				reportResult(TestResultPtr(result));
				return;
			} else if (start.exactMatch(line)) {
				QString name = start.cap(2);
				if (start.cap(1) == "world") worldName = name;
				if (start.cap(1) == "test") testName = name;
				if (start.cap(1) == "suite") suiteName = name;
				TestResultPtr result;
				result = createDefaultResult();
				result->setDescription(name);
				if (start.cap(1) == "test") result->setResult(ResultType::TestStart);
				if (start.cap(1) == "suite") result->setResult(ResultType::TestStart);
				if (start.cap(1) == "world") result->setResult(ResultType::TestStart);
				auto item = result->findTestTreeItem();
				if (item) {
					result->setFileName(item->filePath());
					result->setLine(item->line());
				}
				reportResult(result);
				parents.push(result.data());
				return;
			} else if (completion.exactMatch(line)) {
				QString name = completion.cap(3);
				auto result = createDefaultResult();
				parents.pop();
				if (start.cap(1) == "test") testName.clear();
				if (start.cap(1) == "suite") suiteName.clear();
				if (start.cap(1) == "world") worldName.clear();
				if (completion.cap(2) != "test") return;
				if (completion.cap(1) == "Completed") {
					result->setResult(ResultType::Pass);
					result->setDescription(tr("Successfully completed"));
				}
				if (completion.cap(1) == "Skipped") {
					result->setResult(ResultType::Skip);
					result->setDescription(tr("Skipped"));
				}
				if (completion.cap(1) == "Failed") {
					result->setResult(ResultType::Fail);
					result->setDescription(tr("Failed"));
				}
				auto item = result->findTestTreeItem();
				if (item) {
					result->setFileName(item->filePath());
					result->setLine(item->line());
				}
				reportResult(result);
				return;
			} else if (debug.exactMatch(line)) {
				auto result = createDefaultResult();
				if (debug.cap(1) == "Debug") result->setResult(ResultType::MessageDebug);
				if (debug.cap(1) == "Info") result->setResult(ResultType::MessageInfo);
				if (debug.cap(1) == "Warning") result->setResult(ResultType::MessageWarn);
				if (debug.cap(1) == "Critical") result->setResult(ResultType::MessageWarn);
				if (debug.cap(1) == "Fatal") result->setResult(ResultType::MessageFatal);
				result->setDescription(debug.cap(4));
				result->setFileName(QFileInfo(QDir(m_buildDir), debug.cap(2)).canonicalFilePath());
				result->setLine(debug.cap(3).toInt());
				reportResult(result);
				return;
			} else if (worldIntro.exactMatch(line) || worldOutro.exactMatch(line)) return;
			auto result = createDefaultResult();
			result->setDescription(QString::fromUtf8(outputLineWithNewLine));
			result->setResult(ResultType::MessageSystem);
			reportResult(result);
			qWarning() << "Unprocessed output:" << outputLineWithNewLine;
		}

		TestResultPtr CxxOutputReader::createDefaultResult() const
		{
			return TestResultPtr(new CxxTestResult(id(), worldName, suiteName, testName, currentParent()));
		}

		const TestResult *CxxOutputReader::currentParent() const
		{
			return parents.empty() ? nullptr : parents.top();
		}
	} // namespace Internal
} // namespace Autotest
