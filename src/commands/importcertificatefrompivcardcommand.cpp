/*  commands/importcertificatefrompivcardcommand.cpp

    This file is part of Kleopatra, the KDE keymanager
    SPDX-FileCopyrightText: 2020 g10 Code GmbH
    SPDX-FileContributor: Ingo Klöcker <dev@ingo-kloecker.de>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "importcertificatefrompivcardcommand.h"

#include "importcertificatescommand_p.h"

#include "smartcard/pivcard.h"
#include "smartcard/readerstatus.h"

#include <KLocalizedString>

#include "kleopatra_debug.h"

using namespace Kleo;
using namespace Kleo::Commands;
using namespace Kleo::SmartCard;

class ImportCertificateFromPIVCardCommand::Private : public ImportCertificatesCommand::Private
{
    friend class ::Kleo::Commands::ImportCertificateFromPIVCardCommand;
    ImportCertificateFromPIVCardCommand *q_func() const
    {
        return static_cast<ImportCertificateFromPIVCardCommand *>(q);
    }
public:
    explicit Private(ImportCertificateFromPIVCardCommand *qq,
                     const std::string &slot, const std::string &serialno);
    ~Private();

private:
    void start();

private:
    std::string serialNumber;
    std::string cardSlot;
};

ImportCertificateFromPIVCardCommand::Private *ImportCertificateFromPIVCardCommand::d_func()
{
    return static_cast<Private *>(d.get());
}
const ImportCertificateFromPIVCardCommand::Private *ImportCertificateFromPIVCardCommand::d_func() const
{
    return static_cast<const Private *>(d.get());
}

#define q q_func()
#define d d_func()


ImportCertificateFromPIVCardCommand::Private::Private(ImportCertificateFromPIVCardCommand *qq,
                                                      const std::string &slot, const std::string &serialno)
    : ImportCertificatesCommand::Private(qq, nullptr)
    , serialNumber(serialno)
    , cardSlot(slot)
{
}

ImportCertificateFromPIVCardCommand::Private::~Private()
{
}

void ImportCertificateFromPIVCardCommand::Private::start()
{
    qCDebug(KLEOPATRA_LOG) << "ImportCertificateFromPIVCardCommand::Private::start()";

    const auto pivCard = ReaderStatus::instance()->getCard<PIVCard>(serialNumber);
    if (!pivCard) {
        error(i18n("Failed to find the PIV card with the serial number: %1", QString::fromStdString(serialNumber)));
        finished();
        return;
    }

    const std::string certificateData = pivCard->certificateData(cardSlot);
    if (certificateData.empty()) {
        error(i18n("Sorry! No certificate to import from this card slot was found."));
        finished();
        return;
    }

    startImport(GpgME::CMS, QByteArray::fromStdString(certificateData), i18n("Card Certificate"));
}

ImportCertificateFromPIVCardCommand::ImportCertificateFromPIVCardCommand(const std::string& cardSlot, const std::string &serialno)
    : ImportCertificatesCommand(new Private(this, cardSlot, serialno))
{
}

ImportCertificateFromPIVCardCommand::~ImportCertificateFromPIVCardCommand()
{
    qCDebug(KLEOPATRA_LOG) << "ImportCertificateFromPIVCardCommand::~ImportCertificateFromPIVCardCommand()";
}

void ImportCertificateFromPIVCardCommand::doStart()
{
    qCDebug(KLEOPATRA_LOG) << "ImportCertificateFromPIVCardCommand::doStart()";

    d->start();
}

#undef q_func
#undef d_func
