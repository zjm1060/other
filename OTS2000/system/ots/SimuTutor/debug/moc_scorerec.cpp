/****************************************************************************
** Meta object code from reading C++ file 'scorerec.h'
**
** Created: Fri Jan 23 13:43:58 2015
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/scorerec.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'scorerec.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MyDelegate[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

       0        // eod
};

static const char qt_meta_stringdata_MyDelegate[] = {
    "MyDelegate\0"
};

const QMetaObject MyDelegate::staticMetaObject = {
    { &QItemDelegate::staticMetaObject, qt_meta_stringdata_MyDelegate,
      qt_meta_data_MyDelegate, 0 }
};

const QMetaObject *MyDelegate::metaObject() const
{
    return &staticMetaObject;
}

void *MyDelegate::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MyDelegate))
        return static_cast<void*>(const_cast< MyDelegate*>(this));
    return QItemDelegate::qt_metacast(_clname);
}

int MyDelegate::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QItemDelegate::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
static const uint qt_meta_data_CScoreRec[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // slots: signature, parameters, type, tag, flags
      16,   11,   10,   10, 0x08,
      42,   10,   10,   10, 0x08,
      59,   10,   10,   10, 0x08,
      77,   10,   10,   10, 0x08,
      95,   10,   10,   10, 0x08,
     118,   10,   10,   10, 0x08,
     134,   10,   10,   10, 0x08,
     148,   10,   10,   10, 0x08,
     162,   10,   10,   10, 0x08,
     174,   10,   10,   10, 0x08,
     187,   10,   10,   10, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_CScoreRec[] = {
    "CScoreRec\0\0data\0setScoreRecModel(DB_FMT*)\0"
    "modeChanged(int)\0displayTodayRec()\0"
    "displayScoreRec()\0filterChanged(QString)\0"
    "exportToExcel()\0on_subscore()\0"
    "on_setscore()\0on_Actset()\0on_ActRecd()\0"
    "setLanguage(int)\0"
};

const QMetaObject CScoreRec::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_CScoreRec,
      qt_meta_data_CScoreRec, 0 }
};

const QMetaObject *CScoreRec::metaObject() const
{
    return &staticMetaObject;
}

void *CScoreRec::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CScoreRec))
        return static_cast<void*>(const_cast< CScoreRec*>(this));
    if (!strcmp(_clname, "Ui_CScoreRec"))
        return static_cast< Ui_CScoreRec*>(const_cast< CScoreRec*>(this));
    return QWidget::qt_metacast(_clname);
}

int CScoreRec::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: setScoreRecModel((*reinterpret_cast< DB_FMT*(*)>(_a[1]))); break;
        case 1: modeChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: displayTodayRec(); break;
        case 3: displayScoreRec(); break;
        case 4: filterChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 5: exportToExcel(); break;
        case 6: on_subscore(); break;
        case 7: on_setscore(); break;
        case 8: on_Actset(); break;
        case 9: on_ActRecd(); break;
        case 10: setLanguage((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 11;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
