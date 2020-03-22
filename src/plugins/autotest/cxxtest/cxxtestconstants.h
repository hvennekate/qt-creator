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

#ifndef CXXTESTCONSTANTS_H
#define CXXTESTCONSTANTS_H

#pragma once

#include <QtGlobal>

namespace Autotest {
	namespace CxxTest {
		namespace Constants {
			const char FRAMEWORK_NAME[] = "CxxTest";
			const char FRAMEWORK_SETTINGS_CATEGORY[] = QT_TRANSLATE_NOOP("CxxTestFramework", "Cxx Tests");
			const unsigned FRAMEWORK_PRIORITY = 20;
			const char DEFAULT_FILTER[] = "*.h";
		}
	}
}

#endif // CXXTESTCONSTANTS_H
