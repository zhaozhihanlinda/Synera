#ifndef PLAYERTEMPLATELIBRARY_H
#define PLAYERTEMPLATELIBRARY_H

#include <QVector>

#include "core/unittemplate.h"

QVector<UnitTemplate> allPlayerUnitTemplates();
UnitTemplate playerUnitTemplateById(const QString &templateId);
QVector<UnitTemplate> defaultShopTemplates();
QVector<UnitTemplate> defaultStarterBenchTemplates();

#endif // PLAYERTEMPLATELIBRARY_H
