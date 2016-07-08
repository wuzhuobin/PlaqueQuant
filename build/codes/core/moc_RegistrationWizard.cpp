/****************************************************************************
** Meta object code from reading C++ file 'RegistrationWizard.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../codes/core/RegistrationWizard.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'RegistrationWizard.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_FindImageThread_t {
    QByteArrayData data[7];
    char stringdata0[86];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_FindImageThread_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_FindImageThread_t qt_meta_stringdata_FindImageThread = {
    {
QT_MOC_LITERAL(0, 0, 15), // "FindImageThread"
QT_MOC_LITERAL(1, 16, 10), // "updateTree"
QT_MOC_LITERAL(2, 27, 0), // ""
QT_MOC_LITERAL(3, 28, 16), // "QTreeWidgetItem*"
QT_MOC_LITERAL(4, 45, 12), // "QStringList*"
QT_MOC_LITERAL(5, 58, 17), // "updateProgressBar"
QT_MOC_LITERAL(6, 76, 9) // "forceStop"

    },
    "FindImageThread\0updateTree\0\0"
    "QTreeWidgetItem*\0QStringList*\0"
    "updateProgressBar\0forceStop"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_FindImageThread[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   29,    2, 0x06 /* Public */,
       5,    1,   34,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       6,    0,   37,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 4,    2,    2,
    QMetaType::Void, QMetaType::Int,    2,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

void FindImageThread::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        FindImageThread *_t = static_cast<FindImageThread *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->updateTree((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< QStringList*(*)>(_a[2]))); break;
        case 1: _t->updateProgressBar((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->forceStop(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (FindImageThread::*_t)(QTreeWidgetItem * , QStringList * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&FindImageThread::updateTree)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (FindImageThread::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&FindImageThread::updateProgressBar)) {
                *result = 1;
                return;
            }
        }
    }
}

const QMetaObject FindImageThread::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_FindImageThread.data,
      qt_meta_data_FindImageThread,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *FindImageThread::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *FindImageThread::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_FindImageThread.stringdata0))
        return static_cast<void*>(const_cast< FindImageThread*>(this));
    return QThread::qt_metacast(_clname);
}

int FindImageThread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void FindImageThread::updateTree(QTreeWidgetItem * _t1, QStringList * _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void FindImageThread::updateProgressBar(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
struct qt_meta_stringdata_DirectoryPage_t {
    QByteArrayData data[3];
    char stringdata0[22];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_DirectoryPage_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_DirectoryPage_t qt_meta_stringdata_DirectoryPage = {
    {
QT_MOC_LITERAL(0, 0, 13), // "DirectoryPage"
QT_MOC_LITERAL(1, 14, 6), // "browse"
QT_MOC_LITERAL(2, 21, 0) // ""

    },
    "DirectoryPage\0browse\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_DirectoryPage[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   19,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

void DirectoryPage::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        DirectoryPage *_t = static_cast<DirectoryPage *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->browse(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject DirectoryPage::staticMetaObject = {
    { &QWizardPage::staticMetaObject, qt_meta_stringdata_DirectoryPage.data,
      qt_meta_data_DirectoryPage,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *DirectoryPage::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *DirectoryPage::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_DirectoryPage.stringdata0))
        return static_cast<void*>(const_cast< DirectoryPage*>(this));
    return QWizardPage::qt_metacast(_clname);
}

int DirectoryPage::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWizardPage::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 1;
    }
    return _id;
}
struct qt_meta_stringdata_ImagePage_t {
    QByteArrayData data[16];
    char stringdata0[189];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ImagePage_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ImagePage_t qt_meta_stringdata_ImagePage = {
    {
QT_MOC_LITERAL(0, 0, 9), // "ImagePage"
QT_MOC_LITERAL(1, 10, 12), // "onUpdateTree"
QT_MOC_LITERAL(2, 23, 0), // ""
QT_MOC_LITERAL(3, 24, 16), // "QTreeWidgetItem*"
QT_MOC_LITERAL(4, 41, 12), // "QStringList*"
QT_MOC_LITERAL(5, 54, 19), // "onUpdateProgressBar"
QT_MOC_LITERAL(6, 74, 9), // "setImage1"
QT_MOC_LITERAL(7, 84, 9), // "setImage2"
QT_MOC_LITERAL(8, 94, 9), // "setImage3"
QT_MOC_LITERAL(9, 104, 9), // "setImage4"
QT_MOC_LITERAL(10, 114, 9), // "setImage5"
QT_MOC_LITERAL(11, 124, 12), // "removeImage1"
QT_MOC_LITERAL(12, 137, 12), // "removeImage2"
QT_MOC_LITERAL(13, 150, 12), // "removeImage3"
QT_MOC_LITERAL(14, 163, 12), // "removeImage4"
QT_MOC_LITERAL(15, 176, 12) // "removeImage5"

    },
    "ImagePage\0onUpdateTree\0\0QTreeWidgetItem*\0"
    "QStringList*\0onUpdateProgressBar\0"
    "setImage1\0setImage2\0setImage3\0setImage4\0"
    "setImage5\0removeImage1\0removeImage2\0"
    "removeImage3\0removeImage4\0removeImage5"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ImagePage[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    2,   74,    2, 0x0a /* Public */,
       5,    1,   79,    2, 0x0a /* Public */,
       6,    0,   82,    2, 0x0a /* Public */,
       7,    0,   83,    2, 0x0a /* Public */,
       8,    0,   84,    2, 0x0a /* Public */,
       9,    0,   85,    2, 0x0a /* Public */,
      10,    0,   86,    2, 0x0a /* Public */,
      11,    0,   87,    2, 0x0a /* Public */,
      12,    0,   88,    2, 0x0a /* Public */,
      13,    0,   89,    2, 0x0a /* Public */,
      14,    0,   90,    2, 0x0a /* Public */,
      15,    0,   91,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 4,    2,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void ImagePage::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ImagePage *_t = static_cast<ImagePage *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->onUpdateTree((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< QStringList*(*)>(_a[2]))); break;
        case 1: _t->onUpdateProgressBar((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->setImage1(); break;
        case 3: _t->setImage2(); break;
        case 4: _t->setImage3(); break;
        case 5: _t->setImage4(); break;
        case 6: _t->setImage5(); break;
        case 7: _t->removeImage1(); break;
        case 8: _t->removeImage2(); break;
        case 9: _t->removeImage3(); break;
        case 10: _t->removeImage4(); break;
        case 11: _t->removeImage5(); break;
        default: ;
        }
    }
}

const QMetaObject ImagePage::staticMetaObject = {
    { &QWizardPage::staticMetaObject, qt_meta_stringdata_ImagePage.data,
      qt_meta_data_ImagePage,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *ImagePage::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ImagePage::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_ImagePage.stringdata0))
        return static_cast<void*>(const_cast< ImagePage*>(this));
    return QWizardPage::qt_metacast(_clname);
}

int ImagePage::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWizardPage::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 12)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 12;
    }
    return _id;
}
struct qt_meta_stringdata_ConclusionPage_t {
    QByteArrayData data[1];
    char stringdata0[15];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ConclusionPage_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ConclusionPage_t qt_meta_stringdata_ConclusionPage = {
    {
QT_MOC_LITERAL(0, 0, 14) // "ConclusionPage"

    },
    "ConclusionPage"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ConclusionPage[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

void ConclusionPage::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObject ConclusionPage::staticMetaObject = {
    { &QWizardPage::staticMetaObject, qt_meta_stringdata_ConclusionPage.data,
      qt_meta_data_ConclusionPage,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *ConclusionPage::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ConclusionPage::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_ConclusionPage.stringdata0))
        return static_cast<void*>(const_cast< ConclusionPage*>(this));
    return QWizardPage::qt_metacast(_clname);
}

int ConclusionPage::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWizardPage::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
struct qt_meta_stringdata_RegistrationWizard_t {
    QByteArrayData data[3];
    char stringdata0[29];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_RegistrationWizard_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_RegistrationWizard_t qt_meta_stringdata_RegistrationWizard = {
    {
QT_MOC_LITERAL(0, 0, 18), // "RegistrationWizard"
QT_MOC_LITERAL(1, 19, 8), // "showHelp"
QT_MOC_LITERAL(2, 28, 0) // ""

    },
    "RegistrationWizard\0showHelp\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_RegistrationWizard[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   19,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

void RegistrationWizard::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        RegistrationWizard *_t = static_cast<RegistrationWizard *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->showHelp(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject RegistrationWizard::staticMetaObject = {
    { &QWizard::staticMetaObject, qt_meta_stringdata_RegistrationWizard.data,
      qt_meta_data_RegistrationWizard,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *RegistrationWizard::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *RegistrationWizard::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_RegistrationWizard.stringdata0))
        return static_cast<void*>(const_cast< RegistrationWizard*>(this));
    return QWizard::qt_metacast(_clname);
}

int RegistrationWizard::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWizard::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 1;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
