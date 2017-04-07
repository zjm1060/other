/****************************************************************************
** Meta object code from reading C++ file 'model.h'
**
** Created: Thu Jan 15 10:27:45 2015
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/model.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'model.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CModelAddForm[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // signals: signature, parameters, type, tag, flags
      15,   14,   14,   14, 0x05,
      30,   14,   14,   14, 0x05,

 // slots: signature, parameters, type, tag, flags
      45,   14,   14,   14, 0x08,
      53,   14,   14,   14, 0x08,
      61,   14,   14,   14, 0x08,
      82,   78,   14,   14, 0x08,
      98,   14,   14,   14, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_CModelAddForm[] = {
    "CModelAddForm\0\0insertRecord()\0"
    "modifyRecord()\0apply()\0enter()\0"
    "displayStation()\0stn\0displayDev(int)\0"
    "setLanguage(int)\0"
};

const QMetaObject CModelAddForm::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_CModelAddForm,
      qt_meta_data_CModelAddForm, 0 }
};

const QMetaObject *CModelAddForm::metaObject() const
{
    return &staticMetaObject;
}

void *CModelAddForm::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CModelAddForm))
        return static_cast<void*>(const_cast< CModelAddForm*>(this));
    if (!strcmp(_clname, "Ui_CModelAddForm"))
        return static_cast< Ui_CModelAddForm*>(const_cast< CModelAddForm*>(this));
    return QDialog::qt_metacast(_clname);
}

int CModelAddForm::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: insertRecord(); break;
        case 1: modifyRecord(); break;
        case 2: apply(); break;
        case 3: enter(); break;
        case 4: displayStation(); break;
        case 5: displayDev((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: setLanguage((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void CModelAddForm::insertRecord()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void CModelAddForm::modifyRecord()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}
static const uint qt_meta_data_CModelForm[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
      14,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // slots: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x0a,
      27,   11,   11,   11, 0x0a,
      45,   11,   11,   11, 0x0a,
      62,   11,   11,   11, 0x0a,
      75,   73,   11,   11, 0x0a,
      95,   11,   11,   11, 0x0a,
     106,   11,   11,   11, 0x0a,
     124,   11,  120,   11, 0x0a,
     139,   11,   11,   11, 0x0a,
     157,   11,   11,   11, 0x0a,
     169,   11,   11,   11, 0x0a,
     182,   11,   11,   11, 0x0a,
     201,   73,   11,   11, 0x0a,
     224,   11,   11,   11, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_CModelForm[] = {
    "CModelForm\0\0openModelAdd()\0openModelModify()\0"
    "openModelParam()\0addModel()\0,\0"
    "selectCell(int,int)\0delModel()\0"
    "modifyModel()\0int\0displayModel()\0"
    "openModelConfig()\0intoModel()\0"
    "openStnDev()\0displayUpdate(int)\0"
    "displayUpdate(int,int)\0setLanguage(int)\0"
};

const QMetaObject CModelForm::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_CModelForm,
      qt_meta_data_CModelForm, 0 }
};

const QMetaObject *CModelForm::metaObject() const
{
    return &staticMetaObject;
}

void *CModelForm::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CModelForm))
        return static_cast<void*>(const_cast< CModelForm*>(this));
    if (!strcmp(_clname, "Ui_CModelForm"))
        return static_cast< Ui_CModelForm*>(const_cast< CModelForm*>(this));
    return QDialog::qt_metacast(_clname);
}

int CModelForm::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: openModelAdd(); break;
        case 1: openModelModify(); break;
        case 2: openModelParam(); break;
        case 3: addModel(); break;
        case 4: selectCell((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 5: delModel(); break;
        case 6: modifyModel(); break;
        case 7: { int _r = displayModel();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 8: openModelConfig(); break;
        case 9: intoModel(); break;
        case 10: openStnDev(); break;
        case 11: displayUpdate((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 12: displayUpdate((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 13: setLanguage((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 14;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
