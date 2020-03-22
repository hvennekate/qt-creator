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

#ifndef CXXTESTFRAMEWORK_H
#define CXXTESTFRAMEWORK_H

#include <autotest/itestframework.h>

namespace Autotest {
	namespace Internal {

		class CxxTestSettings;
		class CxxTestSettingsPage;

		class CxxTestFramework : public ITestFramework
		{
		public:
			CxxTestFramework();

		private:
			const char *name() const override;
			unsigned priority() const override;
			IFrameworkSettings *frameworkSettings() override;
			QString groupingToolTip() const override;
			ITestParser *createTestParser() override;
			TestTreeItem *createRootNode() const override;

			QScopedPointer<CxxTestSettings> m_settings;
			QScopedPointer<CxxTestSettingsPage> m_settingsPage;
		};

	} // namespace Internal
} // namespace Autotest

#endif // CXXTESTFRAMEWORK_H
