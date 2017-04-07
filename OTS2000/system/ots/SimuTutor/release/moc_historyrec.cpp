/****************************************************************************
** Meta object code from reading C++ file 'historyrec.h'
**
** Created: Thu Jan 15 10:28:03 2015
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/historyrec.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'historyrec.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_PrintView[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // slots: signature, parameters, type, tag, flags
      19,   11,   10,   10, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_PrintView[] = {
    "PrintView\0\0printer\0print(QPrinter*)\0"
};

const QMetaObject PrintView::staticMetaObject = {
    { &QTableView::staticMetaObject, qt_meta_stringdata_PrintView,
      qt_meta_data_PrintView, 0 }
};

const QMetaObject *PrintView::metaObject() const
{
    return &staticMetaObject;
}

void *PrintView::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_PrintView))
        return static_cast<void*>(const_cast< PrintView*>(this));
    return QTableView::qt_metacast(_clname);
}

int PrintView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QTableView::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: print((*reinterpret_cast< QPrinter*(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 1;
    }
    return _id;
}
static const uint qt_meta_data_CSelectTaskTable[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // slots: signature, parameters, type, tag, flags
      18,   17,   17,   17, 0x08,
      43,   17,   17,   17, 0x08,
      57,   17,   17,   17, 0x08,
      81,   17,   17,   17, 0x08,
     107,   17,  103,   17, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_CSelectTaskTable[] = {
    "CSelectTaskTable\0\0showSendTaskMenu(QPoint)\0"
    "delSendTask()\0delSendTaskFromHdb(int)\0"
    "delRecordFromHdb(int)\0int\0displayTask()\0"
};

const QMetaObject CSelectTaskTable::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_CSelectTaskTable,
      qt_meta_data_CSelectTaskTable, 0 }
};

const QMetaObject *CSelectTaskTable::metaObject() const
{
    return &staticMetaObject;
}

void *CSelectTaskTable::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CSelectTaskTable))
        return static_cast<void*>(const_cast< CSelectTaskTable*>(this));
    if (!strcmp(_clname, "Ui_CSelectTaskTable"))
        return static_cast< Ui_CSelectTaskTable*>(const_cast< CSelectTaskTable*>(this));
    return QDialog::qt_metacast(_clname);
}

int CSelectTaskTable::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: showSendTaskMenu((*reinterpret_cast< const QPoint(*)>(_a[1]))); break;
        case 1: delSendTask(); break;
        case 2: delSendTaskFromHdb((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: delRecordFromHdb((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: { int _r = displayTask();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        default: ;
        }
        _id -= 5;
    }
    return _id;
}
static const uint qt_meta_data_CHistoryRecTable[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
      15,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // slots: signature, parameters, type, tag, flags
      18,   17,   17,   17, 0x0a,
      31,   17,   17,   17, 0x0a,
      49,   17,   17,   17, 0x0a,
      66,   17,   17,   17, 0x08,
      74,   17,   17,   17, 0x08,
      96,   17,   17,   17, 0x08,
     109,   17,   17,   17, 0x08,
     127,   17,   17,   17, 0x08,
     143,   17,   17,   17, 0x08,
     159,   17,   17,   17, 0x08,
     186,   17,   17,   17, 0x08,
     202,   17,   17,   17, 0x08,
     231,   17,   17,   17, 0x08,
     240,   17,   17,   17, 0x08,
     269,   17,   17,   17, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_CHistoryRecTable[] = {
    "CHistoryRecTable\0\0displayRec()\0"
    "displayTodayRec()\0setLanguage(int)\0"
    "print()\0openSelectTaskTable()\0"
    "selectTask()\0displayUserName()\0"
    "displayGroups()\0exportToExcel()\0"
    "setRecSourceModel(DB_FMT*)\0setFilterType()\0"
    "setFilterSearchName(QString)\0delRec()\0"
    "setFilterSearchType(QString)\0delRecord()\0"
};

const QMetaObject CHistoryRecTable::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_CHistoryRecTable,
      qt_meta_data_CHistoryRecTable, 0 }
};

const QMetaObject *CHistoryRecTable::metaObject() const
{
    return &staticMetaObject;
}

void *CHistoryRecTable::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CHistoryRecTable))
        return static_cast<void*>(const_cast< CHistoryRecTable*>(this));
    if (!strcmp(_clname, "Ui_CHistoryRecTable"))
        return static_cast< Ui_CHistoryRecTable*>(const_cast< CHistoryRecTable*>(this));
    return QWidget::qt_metacast(_clname);
}

int CHistoryRecTable::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: displayRec(); break;
        case 1: displayTodayRec(); break;
        case 2: setLanguage((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: print(); break;
        case 4: openSelectTaskTable(); break;
        case 5: selectTask(); break;
        case 6: displayUserName(); break;
        case 7: displayGroups(); break;
        case 8: exportToExcel(); break;
        case 9: setRecSourceModel((*reinterpret_cast< DB_FMT*(*)>(_a[1]))); break;
        case 10: setFilterType(); break;
        case 11: setFilterSearchName((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 12: delRec(); break;
        case 13: setFilterSearchType((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 14: delRecord(); break;
        default: ;
        }
        _id -= 15;
    }
    return _id;
}
static const uint qt_meta_data_CStudyRecTable[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // slots: signature, parameters, type, tag, flags
      16,   15,   15,   15, 0x0a,
      29,   15,   15,   15, 0x0a,
      47,   15,   15,   15, 0x0a,
      64,   15,   15,   15, 0x08,
      72,   15,   15,   15, 0x08,
      90,   15,   15,   15, 0x08,
     105,   15,   15,   15, 0x08,
     121,   15,   15,   15, 0x08,
     148,   15,   15,   15, 0x08,
     164,   15,   15,   15, 0x08,
     193,   15,   15,   15, 0x08,
     202,   15,   15,   15, 0x08,
     231,   15,   15,   15, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_CStudyRecTable[] = {
    "CStudyRecTable\0\0displayRec()\0"
    "displayTodayRec()\0setLanguage(int)\0"
    "print()\0displayUserName()\0displayPlans()\0"
    "exportToExcel()\0setRecSourceModel(DB_FMT*)\0"
    "setFilterType()\0setFilterSearchName(QString)\0"
    "delRec()\0setFilterSearchType(QString)\0"
    "delRecord()\0"
};

const QMetaObject CStudyRecTable::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_CStudyRecTable,
      qt_meta_data_CStudyRecTable, 0 }
};

const QMetaObject *CStudyRecTable::metaObject() const
{
    return &staticMetaObject;
}

void *CStudyRecTable::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CStudyRecTable))
        return static_cast<void*>(const_cast< CStudyRecTable*>(this));
    if (!strcmp(_clname, "Ui_CStudyRecTable"))
        return static_cast< Ui_CStudyRecTable*>(const_cast< CStudyRecTable*>(this));
    return QWidget::qt_metacast(_clname);
}

int CStudyRecTable::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: displayRec(); break;
        case 1: displayTodayRec(); break;
        case 2: setLanguage((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: print(); break;
        case 4: displayUserName(); break;
        case 5: displayPlans(); break;
        case 6: exportToExcel(); break;
        case 7: setRecSourceModel((*reinterpret_cast< DB_FMT*(*)>(_a[1]))); break;
        case 8: setFilterType(); break;
        case 9: setFilterSearchName((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 10: delRec(); break;
        case 11: setFilterSearchType((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 12: delRecord(); break;
        default: ;
        }
        _id -= 13;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
