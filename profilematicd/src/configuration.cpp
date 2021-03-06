/**********************************************************************
 * Copyright 2011 Arto Jalkanen
 *
 * This file is part of ProfileMatic.
 *
 * ProfileMatic is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ProfileMatic is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ProfileMatic.  If not, see <http://www.gnu.org/licenses/>
**/
#include <QSettings>
#include <QUuid>

#include "configuration.h"

Configuration::Configuration()
{
}

void
Configuration::writeRules(const QList<Rule> &rules) {
    qDebug("Configuration::write()");

    // IMPROVE constants.h
    // So wrong by the API specifications, but so right by the end results (no, I don't like doing it this way)
    QSettings s("ProfileMatic", "rules");
    s.clear();

    s.beginWriteArray("rules", rules.size());
    for (int i = 0; i < rules.size(); ++i) {
        s.setArrayIndex(i);

        const Rule &r = rules.at(i);
        // s.setValue("ruleActive", r.ruleActive);

        s.setValue("ruleId", r.getRuleId());
        s.setValue("ruleName", r.getRuleName());
        _writeIntList(s, "days", "dayId", r.getDays().toList());
        s.setValue("timeStart", r.getTimeStart().toString());
        s.setValue("profile", r.getProfile());
        s.setValue("profileVolume", r.getProfileVolume());
        s.setValue("flightMode", r.getFlightMode());
    }
    s.endArray();
}

void
Configuration::readRules(QList<Rule> &rules) {
    // IMPROVE constants.h
    // So wrong by the API specifications, but so right by the end results (no, I don't like doing it this way)
    QSettings s("ProfileMatic", "rules");
    // QSettings s("ajalkane", "ProfileMatic");
    int size = s.beginReadArray("rules");
    for (int i = 0; i < size; ++i) {
        s.setArrayIndex(i);

        Rule r;
        // r.ruleActive = s.value("ruleActive").toBool();

        _assignRuleId(r, s.value("ruleId"));
        r.setRuleName(s.value("ruleName").toString());
        QList<int> daysList;
        _readIntList(s, "days", "dayId", daysList);
        r.setDays(QSet<int>::fromList(daysList));
        QString timeStartStr = s.value("timeStart").toString();
        if (!timeStartStr.isEmpty()) {
            QTime time = QTime::fromString(timeStartStr);
            r.setTimeStart(time);
        }
        r.setProfile(s.value("profile").toString());

        bool profileVolumeOk = false;
        int profileVolume = s.value("profileVolume").toInt(&profileVolumeOk);
        if (profileVolumeOk) {
            r.setProfileVolume(profileVolume);
        }
        bool flightModeOk = false;
        int flightMode = s.value("flightMode").toInt(&flightModeOk);
        if (flightModeOk) {
            r.setFlightMode(flightMode);
        }
        rules << r;
        qDebug("Configuration: index %d, ruleId: %s, ruleName: %s", i, qPrintable(r.getRuleId()), qPrintable(r.getRuleName()));
    }
    s.endArray();
}

void
Configuration::writePreferences(const Preferences &p) {
    qDebug("Configuration::writePreferences()");

    // IMPROVE constants.h
    // So wrong by the API specifications, but so right by the end results (no, I don't like doing it this way)
    QSettings s("ProfileMatic", "preferences");
    s.clear();
    s.setValue("isActive", p.isActive);
}

void
Configuration::readPreferences(Preferences &p) {
    // IMPROVE constants.h
    // So wrong by the API specifications, but so right by the end results (no, I don't like doing it this way)
    QSettings s("ProfileMatic", "rules");
    p.isActive = s.value("isActive", true).toBool();
}

void
Configuration::_assignRuleId(Rule &r, const QVariant &ruleIdVar) {
    // First released version did not set ruleIds, and it was assumed to be int.
    // Now it is changed to be UUID, a string. If the ruleId is a string, create
    // a new UUID
    bool ok = true;
    int oldIntId = ruleIdVar.toInt(&ok);
    QString ruleId;
    if (ok) {
        ruleId = QUuid::createUuid().toString();
        qDebug("ruleId was in old format (%d), creating UUID %s", oldIntId, qPrintable(ruleId));
    } else {
        ruleId = ruleIdVar.toString();
    }
    r.setRuleId(ruleId);
}

void
Configuration::_writeIntList(QSettings &s, const QString &prefix, const QString &key, const QList<int> &values) {
    qDebug("QSettings:__writeIntList prefix %s, key %s, size %d", qPrintable(prefix), qPrintable(key), values.size());
    s.beginWriteArray(prefix, values.size());
    for (int i = 0; i < values.size(); ++i) {
        qDebug("Writing prefix %s, key %s for index %d, size %d", qPrintable(prefix), qPrintable(key), i, values.size());
        s.setArrayIndex(i);
        s.setValue(key, values.at(i));
    }
    s.endArray();
}

void
Configuration::_readIntList(QSettings &s, const QString &prefix, const QString &key, QList<int> &values) {
    int size = s.beginReadArray(prefix);
    for (int i = 0; i < size; ++i) {
        s.setArrayIndex(i);
        bool ok = false;
        QString valueStr = s.value(key).toString();
        int value = valueStr.toInt(&ok);
        if (ok) {
            values << value;
        } else {
            qDebug("Configuration: invalid setting (not an int) at %s/%d/%s: %s",
                   qPrintable(prefix), i, qPrintable(key), qPrintable(valueStr));
        }
    }
    s.endArray();
}
