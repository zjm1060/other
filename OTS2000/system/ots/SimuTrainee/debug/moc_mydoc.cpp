/****************************************************************************
** Meta object code from reading C++ file 'mydoc.h'
**
** Created: Thu Jan 22 17:27:38 2015
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/mydoc.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mydoc.h' doesn't include <QObject>."
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
static const uint qt_meta_data_Mydoc[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // signals: signature, parameters, type, tag, flags
       7,    6,    6,    6, 0x05,

 // slots: signature, parameters, type, tag, flags
      25,   19,    6,    6, 0x08,
      60,    6,    6,    6, 0x08,
      89,    6,    6,    6, 0x08,
     124,    6,    6,    6, 0x08,
     158,    6,    6,    6, 0x08,
     172,    6,    6,    6, 0x08,
     183,    6,    6,    6, 0x08,
     200,    6,    6,    6, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_Mydoc[] = {
    "Mydoc\0\0sig_myrec()\0index\0"
    "on_view_doubleClicked(QModelIndex)\0"
    "on_pushButtonStudy_Clicked()\0"
    "on_pushButton_MystudyRec_Clicked()\0"
    "on_pushButton_finishdoc_Clicked()\0"
    "on_collapse()\0timeplus()\0timeplus_mouse()\0"
    "setLanguage(int)\0"
};

const QMetaObject Mydoc::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_Mydoc,
      qt_meta_data_Mydoc, 0 }
};

const QMetaObject *Mydoc::metaObject() const
{
    return &staticMetaObject;
}

void *Mydoc::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Mydoc))
        return static_cast<void*>(const_cast< Mydoc*>(this));
    if (!strcmp(_clname, "Ui_Mydoc"))
        return static_cast< Ui_Mydoc*>(const_cast< Mydoc*>(this));
    return QWidget::qt_metacast(_clname);
}

int Mydoc::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: sig_myrec(); break;
        case 1: on_view_doubleClicked((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 2: on_pushButtonStudy_Clicked(); break;
        case 3: on_pushButton_MystudyRec_Clicked(); break;
        case 4: on_pushButton_finishdoc_Clicked(); break;
        case 5: on_collapse(); break;
        case 6: timeplus(); break;
        case 7: timeplus_mouse(); break;
        case 8: setLanguage((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void Mydoc::sig_myrec()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE
