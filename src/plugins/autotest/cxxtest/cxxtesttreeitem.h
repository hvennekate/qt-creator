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

#ifndef CXXTESTTREEITEM_H
#define CXXTESTTREEITEM_H

#include <autotest/testtreeitem.h>

namespace Autotest {
	namespace Internal {

		class CxxTestTreeItem : public TestTreeItem
		{
		public:
			CxxTestTreeItem(const QString &name = QString(), const QString &filePath = QString(), Type type = Root);
			TestTreeItem *copyWithoutChildren() override;
			QVariant data(int column, int role) const override;
			bool canProvideTestConfiguration() const override;
			bool canProvideDebugConfiguration() const override;
			TestConfiguration *testConfiguration() const override;
			TestConfiguration *debugConfiguration() const override;
			QList<TestConfiguration *> getAllTestConfigurations() const override;
			QList<TestConfiguration *> getSelectedTestConfigurations() const override;
			QList<TestConfiguration *> getTestConfigurationsForFile(const Utils::FilePath &fileName) const override;
			TestTreeItem *find(const TestParseResult *parseResult) override;
			TestTreeItem *findChild(const TestTreeItem *other) override;
			bool modify(const TestParseResult *result) override;
			bool isGroupable() const override;
			TestTreeItem *createParentGroupNode() const override;
		};

	} // namespace Internal
} // namespace Autotest

#endif // CXXTESTTREEITEM_H
