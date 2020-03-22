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

#ifndef CXXTESTRESULT_H
#define CXXTESTRESULT_H

#include "../testresult.h"

namespace Autotest {
	namespace Internal {

		class CxxTestResult : public TestResult
		{
		public:
			CxxTestResult(const QString &id, const QString &world, const QString &suite, const QString &test, const TestResult *parent = nullptr);
			const QString outputString(bool selected) const override;
			const TestTreeItem *findTestTreeItem() const override;
			bool isDirectParentOf(const TestResult *other, bool *needsIntermediate) const override;
		private:
			QString test, suite;
			const TestResult *parent;

		};

	} // namespace Internal
} // namespace Autotest

#endif // CXXTESTRESULT_H
