/****************************************************************************
** Meta object code from reading C++ file 'Login.h'
**
** Created: Wed Jan 14 10:22:46 2015
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/Login.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Login.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CLogin[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // signals: signature, parameters, type, tag, flags
      31,    8,    7,    7, 0x05,

 // slots: signature, parameters, type, tag, flags
      61,    7,    7,    7, 0x08,
      87,    7,    7,    7, 0x08,
      97,    7,    7,    7, 0x08,
     130,  116,  112,    7, 0x08,
     178,  162,    7,    7, 0x08,
     193,    7,    7,    7, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_CLogin[] = {
    "CLogin\0\0user_id,user_name,mode\0"
    "sigUserLogin(int,QString,int)\0"
    "openStudentInfoAddTable()\0addUser()\0"
    "studentLogin()\0int\0name,password\0"
    "checkLoginInfo(QString,QString)\0"
    "current_user_id\0userLogin(int)\0on_exit()\0"
};

const QMetaObject CLogin::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_CLogin,
      qt_meta_data_CLogin, 0 }
};

const QMetaObject *CLogin::metaObject() const
{
    return &staticMetaObject;
}

void *CLogin::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CLogin))
        return static_cast<void*>(const_cast< CLogin*>(this));
    if (!strcmp(_clname, "Ui_Login"))
        return static_cast< Ui_Login*>(const_cast< CLogin*>(this));
    return QDialog::qt_metacast(_clname);
}

int CLogin::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: sigUserLogin((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 1: openStudentInfoAddTable(); break;
        case 2: addUser(); break;
        case 3: studentLogin(); break;
        case 4: { int _r = checkLoginInfo((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 5: userLogin((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: on_exit(); break;
        default: ;
        }
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void CLogin::sigUserLogin(int _t1, const QString & _t2, int _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
