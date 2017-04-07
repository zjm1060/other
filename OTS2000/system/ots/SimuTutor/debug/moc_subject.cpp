/****************************************************************************
** Meta object code from reading C++ file 'subject.h'
**
** Created: Fri Jan 23 13:43:39 2015
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/subject.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'subject.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CSubjectCondition[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
      24,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // signals: signature, parameters, type, tag, flags
      29,   19,   18,   18, 0x05,

 // slots: signature, parameters, type, tag, flags
      58,   18,   18,   18, 0x08,
      79,   18,   18,   18, 0x08,
      98,   18,   18,   18, 0x08,
     129,  119,   18,   18, 0x08,
     156,  119,   18,   18, 0x08,
     181,  119,   18,   18, 0x08,
     212,  208,   18,   18, 0x08,
     235,  208,   18,   18, 0x08,
     256,  208,   18,   18, 0x08,
     279,  208,   18,   18, 0x08,
     300,   18,   18,   18, 0x08,
     321,  315,   18,   18, 0x08,
     345,  315,   18,   18, 0x08,
     365,  315,   18,   18, 0x08,
     385,   18,   18,   18, 0x08,
     406,  401,   18,   18, 0x08,
     430,   18,   18,   18, 0x08,
     451,  447,   18,   18, 0x08,
     482,  471,   18,   18, 0x08,
     508,   18,   18,   18, 0x08,
     526,   18,   18,   18, 0x08,
     541,   18,   18,   18, 0x08,
     548,   18,   18,   18, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_CSubjectCondition[] = {
    "CSubjectCondition\0\0condition\0"
    "sigConditionChanged(QString)\0"
    "openPointNameAlarm()\0openPointNameCmd()\0"
    "openPointNameCheck()\0pointName\0"
    "setPointNameAlarm(QString)\0"
    "setPointNameCmd(QString)\0"
    "setPointNameCheck(QString)\0row\0"
    "setConditionAlarm(int)\0setConditionCmd(int)\0"
    "setConditionCheck(int)\0setConditionOpr(int)\0"
    "delCondition()\0index\0alarmSymbolChanged(int)\0"
    "cmdTypeChanged(int)\0oprTypeChanged(int)\0"
    "displaySymbol()\0text\0displayCmdType(QString)\0"
    "displayOprType()\0pos\0openMenuDel(QPoint)\0"
    "row,column\0restoreCondition(int,int)\0"
    "modifyCondition()\0addCondition()\0"
    "save()\0setLanguage(int)\0"
};

const QMetaObject CSubjectCondition::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_CSubjectCondition,
      qt_meta_data_CSubjectCondition, 0 }
};

const QMetaObject *CSubjectCondition::metaObject() const
{
    return &staticMetaObject;
}

void *CSubjectCondition::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CSubjectCondition))
        return static_cast<void*>(const_cast< CSubjectCondition*>(this));
    if (!strcmp(_clname, "Ui_CSubjectCondition"))
        return static_cast< Ui_CSubjectCondition*>(const_cast< CSubjectCondition*>(this));
    return QDialog::qt_metacast(_clname);
}

int CSubjectCondition::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: sigConditionChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: openPointNameAlarm(); break;
        case 2: openPointNameCmd(); break;
        case 3: openPointNameCheck(); break;
        case 4: setPointNameAlarm((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 5: setPointNameCmd((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 6: setPointNameCheck((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 7: setConditionAlarm((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: setConditionCmd((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: setConditionCheck((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 10: setConditionOpr((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 11: delCondition(); break;
        case 12: alarmSymbolChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 13: cmdTypeChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 14: oprTypeChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 15: displaySymbol(); break;
        case 16: displayCmdType((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 17: displayOprType(); break;
        case 18: openMenuDel((*reinterpret_cast< const QPoint(*)>(_a[1]))); break;
        case 19: restoreCondition((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 20: modifyCondition(); break;
        case 21: addCondition(); break;
        case 22: save(); break;
        case 23: setLanguage((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 24;
    }
    return _id;
}

// SIGNAL 0
void CSubjectCondition::sigConditionChanged(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
static const uint qt_meta_data_CSubjectEdit[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // slots: signature, parameters, type, tag, flags
      14,   13,   13,   13, 0x08,
      37,   13,   13,   13, 0x08,
      73,   63,   13,   13, 0x08,
     102,   63,   13,   13, 0x08,
     134,   13,   13,   13, 0x08,
     141,   13,   13,   13, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_CSubjectEdit[] = {
    "CSubjectEdit\0\0openSubjectCondition()\0"
    "openPreSubjectCondition()\0condition\0"
    "setSubjectCondition(QString)\0"
    "setPreSubjectCondition(QString)\0save()\0"
    "setLanguage(int)\0"
};

const QMetaObject CSubjectEdit::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_CSubjectEdit,
      qt_meta_data_CSubjectEdit, 0 }
};

const QMetaObject *CSubjectEdit::metaObject() const
{
    return &staticMetaObject;
}

void *CSubjectEdit::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CSubjectEdit))
        return static_cast<void*>(const_cast< CSubjectEdit*>(this));
    if (!strcmp(_clname, "Ui_CSubjectEdit"))
        return static_cast< Ui_CSubjectEdit*>(const_cast< CSubjectEdit*>(this));
    return QDialog::qt_metacast(_clname);
}

int CSubjectEdit::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: openSubjectCondition(); break;
        case 1: openPreSubjectCondition(); break;
        case 2: setSubjectCondition((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: setPreSubjectCondition((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 4: save(); break;
        case 5: setLanguage((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 6;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
