/*
   SPDX-FileCopyrightText: 2021 Alexander Lohnau <alexander.lohnau@gmx.de>
   SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "gnupgsystemconfigurationpage.h"
#include <KPluginFactory>

K_PLUGIN_FACTORY(GnuPGSystemConfigurationPageFactory, registerPlugin<Kleo::Config::GnuPGSystemConfigurationPage>();)

#include "kcm_gnupgsystemconfigurationpage.moc"
