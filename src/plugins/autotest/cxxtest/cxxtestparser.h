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

#ifndef CXXTESTPARSER_H
#define CXXTESTPARSER_H

#include <autotest/itestparser.h>

namespace Autotest {
	namespace Internal {

		class CxxTestTreeItem;

		class CxxTestParseResult : public TestParseResult
		{
		public:
			explicit CxxTestParseResult(ITestFramework *fw);
			TestTreeItem *createTestTreeItem() const override;
			bool matches(const TestTreeItem *item) const;
		};

		class CxxTestParser : public CppParser
		{
		private:
			QSet<QString> parentTestClasses(const QString &fileName, QSet<QString> visitedFileNames) const;
			CxxTestParseResult *generateClassParseResult(CPlusPlus::Symbol *symbol, QString proFile, const QString &fileName);
			CxxTestParseResult *prepareTestParseResult(const QString &fileName, const QString &proFile, CPlusPlus::Symbol *symbol, TestTreeItem::Type type);

		public:
			explicit CxxTestParser(ITestFramework *framework) : CppParser(framework) {}
			bool processDocument(QFutureInterface<TestParseResultPtr> futureInterface, const QString &fileName) override;
		};

	} // namespace Internal
} // namespace Autotest

#endif // CXXTESTPARSER_H
