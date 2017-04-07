/****************************************************************************
** Meta object code from reading C++ file 'doc_manage.h'
**
** Created: Thu Jan 15 10:28:15 2015
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/doc_manage.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'doc_manage.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MytreeModel[] = {

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

static const char qt_meta_stringdata_MytreeModel[] = {
    "MytreeModel\0"
};

const QMetaObject MytreeModel::staticMetaObject = {
    { &QAbstractItemModel::staticMetaObject, qt_meta_stringdata_MytreeModel,
      qt_meta_data_MytreeModel, 0 }
};

const QMetaObject *MytreeModel::metaObject() const
{
    return &staticMetaObject;
}

void *MytreeModel::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MytreeModel))
        return static_cast<void*>(const_cast< MytreeModel*>(this));
    return QAbstractItemModel::qt_metacast(_clname);
}

int MytreeModel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QAbstractItemModel::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
static const uint qt_meta_data_Doc_manage[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // slots: signature, parameters, type, tag, flags
      18,   12,   11,   11, 0x08,
      49,   11,   11,   11, 0x08,
      72,   11,   11,   11, 0x08,
      99,   11,   11,   11, 0x08,
     125,   11,   11,   11, 0x08,
     148,   11,   11,   11, 0x08,
     167,   11,   11,   11, 0x08,
     190,   11,   11,   11, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_Doc_manage[] = {
    "Doc_manage\0\0index\0on_view_DBclicked(QModelIndex)\0"
    "on_pushButton_adddoc()\0"
    "on_pushButton_LoadAlldoc()\0"
    "on_pushButton_modifydoc()\0"
    "on_pushButton_deldoc()\0on_pushButton_OK()\0"
    "on_pushButton_CANCEL()\0setLanguage(int)\0"
};

const QMetaObject Doc_manage::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_Doc_manage,
      qt_meta_data_Doc_manage, 0 }
};

const QMetaObject *Doc_manage::metaObject() const
{
    return &staticMetaObject;
}

void *Doc_manage::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Doc_manage))
        return static_cast<void*>(const_cast< Doc_manage*>(this));
    if (!strcmp(_clname, "Ui_doc_manage"))
        return static_cast< Ui_doc_manage*>(const_cast< Doc_manage*>(this));
    return QDialog::qt_metacast(_clname);
}

int Doc_manage::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: on_view_DBclicked((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 1: on_pushButton_adddoc(); break;
        case 2: on_pushButton_LoadAlldoc(); break;
        case 3: on_pushButton_modifydoc(); break;
        case 4: on_pushButton_deldoc(); break;
        case 5: on_pushButton_OK(); break;
        case 6: on_pushButton_CANCEL(); break;
        case 7: setLanguage((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 8;
    }
    return _id;
}
static const uint qt_meta_data_Study_plan[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // slots: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x08,
      36,   11,   11,   11, 0x08,
      63,   11,   11,   11, 0x08,
      87,   11,   11,   11, 0x08,
     106,   11,   11,   11, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_Study_plan[] = {
    "Study_plan\0\0on_pushButton_addplan()\0"
    "on_pushButton_modifyplan()\0"
    "on_pushButton_delplan()\0on_pushButton_OK()\0"
    "on_pushButton_EXIT()\0"
};

const QMetaObject Study_plan::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_Study_plan,
      qt_meta_data_Study_plan, 0 }
};

const QMetaObject *Study_plan::metaObject() const
{
    return &staticMetaObject;
}

void *Study_plan::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Study_plan))
        return static_cast<void*>(const_cast< Study_plan*>(this));
    if (!strcmp(_clname, "Ui_study_plan"))
        return static_cast< Ui_study_plan*>(const_cast< Study_plan*>(this));
    return QDialog::qt_metacast(_clname);
}

int Study_plan::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: on_pushButton_addplan(); break;
        case 1: on_pushButton_modifyplan(); break;
        case 2: on_pushButton_delplan(); break;
        case 3: on_pushButton_OK(); break;
        case 4: on_pushButton_EXIT(); break;
        default: ;
        }
        _id -= 5;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
