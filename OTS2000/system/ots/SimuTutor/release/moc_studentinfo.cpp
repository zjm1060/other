/****************************************************************************
** Meta object code from reading C++ file 'studentinfo.h'
**
** Created: Thu Jan 15 10:25:23 2015
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/studentinfo.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'studentinfo.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CGroupsTable[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
      15,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // slots: signature, parameters, type, tag, flags
      14,   13,   13,   13, 0x08,
      48,   13,   13,   13, 0x08,
      69,   13,   13,   13, 0x08,
      93,   13,   13,   13, 0x08,
     105,   13,   13,   13, 0x08,
     117,   13,   13,   13, 0x08,
     144,   13,   13,   13, 0x08,
     165,   13,   13,   13, 0x08,
     181,   13,   13,   13, 0x08,
     197,   13,   13,   13, 0x08,
     213,   13,   13,   13, 0x08,
     228,   13,   13,   13, 0x08,
     254,   13,   13,   13, 0x08,
     272,   13,   13,   13, 0x08,
     299,   13,   13,   13, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_CGroupsTable[] = {
    "CGroupsTable\0\0showGroupsUsers(QListWidgetItem*)\0"
    "openAddGroupsTable()\0openModifyGroupsTable()\0"
    "addGroups()\0delGroups()\0"
    "addGroupsUser(QModelIndex)\0"
    "addAllNoGroupsUser()\0chgUserGroups()\0"
    "delGroupsUser()\0setGroupsHead()\0"
    "modifyGroups()\0menuListGroupUser(QPoint)\0"
    "openNoGroupUser()\0filterNoGroupUser(QString)\0"
    "setLanguage(int)\0"
};

const QMetaObject CGroupsTable::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_CGroupsTable,
      qt_meta_data_CGroupsTable, 0 }
};

const QMetaObject *CGroupsTable::metaObject() const
{
    return &staticMetaObject;
}

void *CGroupsTable::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CGroupsTable))
        return static_cast<void*>(const_cast< CGroupsTable*>(this));
    if (!strcmp(_clname, "Ui_CGroupsTable"))
        return static_cast< Ui_CGroupsTable*>(const_cast< CGroupsTable*>(this));
    return QDialog::qt_metacast(_clname);
}

int CGroupsTable::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: showGroupsUsers((*reinterpret_cast< QListWidgetItem*(*)>(_a[1]))); break;
        case 1: openAddGroupsTable(); break;
        case 2: openModifyGroupsTable(); break;
        case 3: addGroups(); break;
        case 4: delGroups(); break;
        case 5: addGroupsUser((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 6: addAllNoGroupsUser(); break;
        case 7: chgUserGroups(); break;
        case 8: delGroupsUser(); break;
        case 9: setGroupsHead(); break;
        case 10: modifyGroups(); break;
        case 11: menuListGroupUser((*reinterpret_cast< const QPoint(*)>(_a[1]))); break;
        case 12: openNoGroupUser(); break;
        case 13: filterNoGroupUser((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 14: setLanguage((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 15;
    }
    return _id;
}
static const uint qt_meta_data_CStudentInfoAddTable[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // signals: signature, parameters, type, tag, flags
      22,   21,   21,   21, 0x05,
      37,   21,   21,   21, 0x05,

 // slots: signature, parameters, type, tag, flags
      56,   21,   52,   21, 0x08,
      64,   21,   21,   21, 0x08,
      74,   72,   21,   21, 0x08,
     101,   21,   21,   21, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_CStudentInfoAddTable[] = {
    "CStudentInfoAddTable\0\0insertRecord()\0"
    "modifyRecord()\0int\0apply()\0enter()\0i\0"
    "range_setCurrentIndex(int)\0setLanguage(int)\0"
};

const QMetaObject CStudentInfoAddTable::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_CStudentInfoAddTable,
      qt_meta_data_CStudentInfoAddTable, 0 }
};

const QMetaObject *CStudentInfoAddTable::metaObject() const
{
    return &staticMetaObject;
}

void *CStudentInfoAddTable::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CStudentInfoAddTable))
        return static_cast<void*>(const_cast< CStudentInfoAddTable*>(this));
    if (!strcmp(_clname, "Ui_CStudentInfoAddTable"))
        return static_cast< Ui_CStudentInfoAddTable*>(const_cast< CStudentInfoAddTable*>(this));
    return QDialog::qt_metacast(_clname);
}

int CStudentInfoAddTable::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: insertRecord(); break;
        case 1: modifyRecord(); break;
        case 2: { int _r = apply();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 3: enter(); break;
        case 4: range_setCurrentIndex((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: setLanguage((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void CStudentInfoAddTable::insertRecord()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void CStudentInfoAddTable::modifyRecord()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}
static const uint qt_meta_data_CStudentInfoTable[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
      16,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // slots: signature, parameters, type, tag, flags
      19,   18,   18,   18, 0x0a,
      48,   18,   18,   18, 0x0a,
      74,   18,   18,   18, 0x0a,
     103,   18,   18,   18, 0x0a,
     121,   18,   18,   18, 0x0a,
     131,   18,   18,   18, 0x0a,
     144,   18,   18,   18, 0x0a,
     170,   18,   18,   18, 0x0a,
     194,   18,   18,   18, 0x0a,
     218,   18,   18,   18, 0x0a,
     233,   18,   18,   18, 0x0a,
     259,   18,   18,   18, 0x0a,
     282,   18,   18,   18, 0x0a,
     304,   18,   18,   18, 0x0a,
     314,   18,   18,   18, 0x0a,
     325,   18,   18,   18, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_CStudentInfoTable[] = {
    "CStudentInfoTable\0\0openStudentInfoLookAtTable()\0"
    "openStudentInfoAddTable()\0"
    "openStudentInfoModifyTable()\0"
    "openGroupsTable()\0addUser()\0modifyUser()\0"
    "displayOperateRecord(int)\0"
    "displayAlarmRecord(int)\0displayLoginRecord(int)\0"
    "setUserModel()\0changeNameFilter(QString)\0"
    "setNameFilter(QString)\0filterButtonClicked()\0"
    "delUser()\0resetPwd()\0setLanguage(int)\0"
};

const QMetaObject CStudentInfoTable::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_CStudentInfoTable,
      qt_meta_data_CStudentInfoTable, 0 }
};

const QMetaObject *CStudentInfoTable::metaObject() const
{
    return &staticMetaObject;
}

void *CStudentInfoTable::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CStudentInfoTable))
        return static_cast<void*>(const_cast< CStudentInfoTable*>(this));
    if (!strcmp(_clname, "Ui_CStudentInfoTable"))
        return static_cast< Ui_CStudentInfoTable*>(const_cast< CStudentInfoTable*>(this));
    return QDialog::qt_metacast(_clname);
}

int CStudentInfoTable::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: openStudentInfoLookAtTable(); break;
        case 1: openStudentInfoAddTable(); break;
        case 2: openStudentInfoModifyTable(); break;
        case 3: openGroupsTable(); break;
        case 4: addUser(); break;
        case 5: modifyUser(); break;
        case 6: displayOperateRecord((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: displayAlarmRecord((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: displayLoginRecord((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: setUserModel(); break;
        case 10: changeNameFilter((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 11: setNameFilter((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 12: filterButtonClicked(); break;
        case 13: delUser(); break;
        case 14: resetPwd(); break;
        case 15: setLanguage((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 16;
    }
    return _id;
}
static const uint qt_meta_data_ComboBoxDelegate[] = {

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

static const char qt_meta_stringdata_ComboBoxDelegate[] = {
    "ComboBoxDelegate\0"
};

const QMetaObject ComboBoxDelegate::staticMetaObject = {
    { &QItemDelegate::staticMetaObject, qt_meta_stringdata_ComboBoxDelegate,
      qt_meta_data_ComboBoxDelegate, 0 }
};

const QMetaObject *ComboBoxDelegate::metaObject() const
{
    return &staticMetaObject;
}

void *ComboBoxDelegate::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ComboBoxDelegate))
        return static_cast<void*>(const_cast< ComboBoxDelegate*>(this));
    return QItemDelegate::qt_metacast(_clname);
}

int ComboBoxDelegate::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QItemDelegate::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
QT_END_MOC_NAMESPACE
