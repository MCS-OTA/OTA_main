/****************************************************************************
** Meta object code from reading C++ file 'ic_interstubimpl.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../ic_interstubimpl.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ic_interstubimpl.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_IC_interStubImpl_t {
    QByteArrayData data[6];
    char stringdata0[74];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_IC_interStubImpl_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_IC_interStubImpl_t qt_meta_stringdata_IC_interStubImpl = {
    {
QT_MOC_LITERAL(0, 0, 16), // "IC_interStubImpl"
QT_MOC_LITERAL(1, 17, 16), // "signalGear_inter"
QT_MOC_LITERAL(2, 34, 0), // ""
QT_MOC_LITERAL(3, 35, 11), // "std::string"
QT_MOC_LITERAL(4, 47, 18), // "signalLrsign_inter"
QT_MOC_LITERAL(5, 66, 7) // "int32_t"

    },
    "IC_interStubImpl\0signalGear_inter\0\0"
    "std::string\0signalLrsign_inter\0int32_t"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_IC_interStubImpl[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   24,    2, 0x06 /* Public */,
       4,    1,   27,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    2,
    QMetaType::Void, 0x80000000 | 5,    2,

       0        // eod
};

void IC_interStubImpl::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<IC_interStubImpl *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->signalGear_inter((*reinterpret_cast< std::string(*)>(_a[1]))); break;
        case 1: _t->signalLrsign_inter((*reinterpret_cast< int32_t(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (IC_interStubImpl::*)(std::string );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&IC_interStubImpl::signalGear_inter)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (IC_interStubImpl::*)(int32_t );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&IC_interStubImpl::signalLrsign_inter)) {
                *result = 1;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject IC_interStubImpl::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_IC_interStubImpl.data,
    qt_meta_data_IC_interStubImpl,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *IC_interStubImpl::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *IC_interStubImpl::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_IC_interStubImpl.stringdata0))
        return static_cast<void*>(this);
    if (!strcmp(_clname, "v0::commonapi::IC_interStubDefault"))
        return static_cast< v0::commonapi::IC_interStubDefault*>(this);
    return QObject::qt_metacast(_clname);
}

int IC_interStubImpl::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void IC_interStubImpl::signalGear_inter(std::string _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void IC_interStubImpl::signalLrsign_inter(int32_t _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
