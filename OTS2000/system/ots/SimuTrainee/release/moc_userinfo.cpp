/****************************************************************************
** Meta object code from reading C++ file 'userinfo.h'
**
** Created: Wed Jan 14 10:22:07 2015
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/userinfo.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'userinfo.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_UserInfo[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // signals: signature, parameters, type, tag, flags
      10,    9,    9,    9, 0x05,

 // slots: signature, parameters, type, tag, flags
      21,    9,    9,    9, 0x0a,
      36,    9,   32,    9, 0x0a,
      44,    9,    9,    9, 0x0a,
      52,    9,    9,    9, 0x0a,
      62,    9,    9,    9, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_UserInfo[] = {
    "UserInfo\0\0closedlg()\0resetPwd()\0int\0"
    "apply()\0enter()\0onclose()\0setLanguage(int)\0"
};

const QMetaObject UserInfo::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_UserInfo,
      qt_meta_data_UserInfo, 0 }
};

const QMetaObject *UserInfo::metaObject() const
{
    return &staticMetaObject;
}

void *UserInfo::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_UserInfo))
        return static_cast<void*>(const_cast< UserInfo*>(this));
    if (!strcmp(_clname, "Ui_userinfo"))
        return static_cast< Ui_userinfo*>(const_cast< UserInfo*>(this));
    return QWidget::qt_metacast(_clname);
}

int UserInfo::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: closedlg(); break;
        case 1: resetPwd(); break;
        case 2: { int _r = apply();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 3: enter(); break;
        case 4: onclose(); break;
        case 5: setLanguage((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void UserInfo::closedlg()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
static const uint qt_meta_data_CChangePassword[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // signals: signature, parameters, type, tag, flags
      21,   17,   16,   16, 0x05,
      43,   17,   16,   16, 0x05,
      65,   17,   16,   16, 0x05,

 // slots: signature, parameters, type, tag, flags
      94,   16,   16,   16, 0x08,
     102,   17,   16,   16, 0x08,
     118,   17,   16,   16, 0x08,
     134,   17,   16,   16, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_CChangePassword[] = {
    "CChangePassword\0\0str\0sigUpdateOld(QString)\0"
    "sigUpdateNew(QString)\0"
    "sigUpdateNewConfirm(QString)\0enter()\0"
    "getOld(QString)\0getNew(QString)\0"
    "getNewConfirm(QString)\0"
};

const QMetaObject CChangePassword::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_CChangePassword,
      qt_meta_data_CChangePassword, 0 }
};

const QMetaObject *CChangePassword::metaObject() const
{
    return &staticMetaObject;
}

void *CChangePassword::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CChangePassword))
        return static_cast<void*>(const_cast< CChangePassword*>(this));
    if (!strcmp(_clname, "Ui_ChangePassword"))
        return static_cast< Ui_ChangePassword*>(const_cast< CChangePassword*>(this));
    return QDialog::qt_metacast(_clname);
}

int CChangePassword::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: sigUpdateOld((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: sigUpdateNew((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: sigUpdateNewConfirm((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: enter(); break;
        case 4: getOld((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 5: getNew((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 6: getNewConfirm((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void CChangePassword::sigUpdateOld(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CChangePassword::sigUpdateNew(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void CChangePassword::sigUpdateNewConfirm(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE
