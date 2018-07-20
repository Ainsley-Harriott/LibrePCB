/*
 * LibrePCB - Professional EDA for everyone!
 * Copyright (C) 2013 LibrePCB Developers, see AUTHORS.md for contributors.
 * http://librepcb.org/
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef LIBREPCB_SEXPRESSION_H
#define LIBREPCB_SEXPRESSION_H

/*****************************************************************************************
 *  Includes
 ****************************************************************************************/
#include <QtCore>
#include <QtWidgets>
#include <optional/tl/optional.hpp>
#include "filepath.h"
#include "../exceptions.h"

/*****************************************************************************************
 *  Namespace / Forward Declarations
 ****************************************************************************************/
namespace sexpresso {
struct Sexp;
}

namespace librepcb {

/*****************************************************************************************
 *  Class SExpression
 ****************************************************************************************/

/**
 * @brief The SExpression class
 *
 * @author ubruhin
 * @date 2017-10-17
 */
class SExpression final
{
        Q_DECLARE_TR_FUNCTIONS(SExpression)

    public:

        // Types
        enum class Type {
            List,       ///< has a tag name and an arbitrary number of children
            Token,      ///< values without quotes (e.g. `-12.34`)
            String,     ///< values with double quotes (e.g. `"Foo!"`)
            LineBreak,  ///< manual line break inside a List
        };

        // Constructors / Destructor
        SExpression() noexcept;
        SExpression(const SExpression& other) noexcept;
        ~SExpression() noexcept;

        // Getters
        const FilePath& getFilePath() const noexcept {return mFilePath;}
        Type getType() const noexcept {return mType;}
        bool isList() const noexcept {return mType == Type::List;}
        bool isToken() const noexcept {return mType == Type::Token;}
        bool isString() const noexcept {return mType == Type::String;}
        bool isLineBreak() const noexcept {return mType == Type::LineBreak;}
        bool isMultiLineList() const noexcept;
        const QString& getName() const;
        const QList<SExpression>& getChildren() const {return mChildren;}
        QList<SExpression> getChildren(const QString& name) const noexcept;
        const SExpression& getChildByIndex(int index) const;
        const SExpression* tryGetChildByPath(const QString& path) const noexcept;
        const SExpression& getChildByPath(const QString& path) const;

        template <typename T>
        T getValue(bool throwIfEmpty = false) const
        {
            try {
                if (!isToken() && !isString()) {
                    throw RuntimeError(__FILE__, __LINE__, tr("Node is not a token or string."));
                }
                if (mValue.isEmpty() && throwIfEmpty) {
                    throw RuntimeError(__FILE__, __LINE__, tr("Node value is empty."));
                }
                return stringToObject(mValue, static_cast<T*>(nullptr));
            } catch (const Exception& e) {
                throw FileParseError(__FILE__, __LINE__, mFilePath, -1, -1, mValue, e.getMsg());
            }
        }

        template <typename T>
        T getValueByPath(const QString& path, bool throwIfEmpty = false) const
        {
            const SExpression& child = getChildByPath(path);
            return child.getValueOfFirstChild<T>(throwIfEmpty);
        }

        template <typename T>
        T getValueOfFirstChild(bool throwIfEmpty = false) const
        {
            if (mChildren.count() < 1) {
                throw FileParseError(__FILE__, __LINE__, mFilePath, -1, -1, QString(),
                                     tr("Node does not have children."));
            }
            return mChildren.at(0).getValue<T>(throwIfEmpty);
        }


        // General Methods
        template <typename T>
        SExpression& appendTokenChild(const QString& child, const T& token, bool linebreak) {
            return appendList(child, linebreak).appendToken<T>(token);
        }
        template <typename T>
        SExpression& appendStringChild(const QString& child, const T& string, bool linebreak) {
            return appendList(child, linebreak).appendString<T>(string);
        }
        template <typename T>
        SExpression& appendToken(const T& token) {
            appendChild(createToken(objectToString(token)), false);
            return *this;
        }
        template <typename T>
        SExpression& appendString(const T& string) {
            appendChild(createString(objectToString(string)), false);
            return *this;
        }
        SExpression& appendLineBreak();
        SExpression& appendList(const QString& name, bool linebreak);
        SExpression& appendChild(const SExpression& child, bool linebreak);
        void removeLineBreaks() noexcept;
        QString toString(int indent) const;

        // Operator Overloadings
        SExpression& operator=(const SExpression& rhs) noexcept;

        // Static Methods
        static SExpression createList(const QString& name);
        static SExpression createToken(const QString& token);
        static SExpression createString(const QString& string);
        static SExpression createLineBreak();
        static SExpression parse(const QString& str, const FilePath& filePath);


    private: // Methods
        SExpression(Type type, const QString& value);
        SExpression(sexpresso::Sexp& sexp, const FilePath& filePath);

        QString escapeString(const QString& string) const noexcept;
        bool isValidListName(const QString& name) const noexcept;
        bool isValidToken(const QString& token) const noexcept;

        /**
         * @brief Serialization template method
         *
         * @tparam T    Type of the object to be serialized
         *
         * @param obj   Input object
         *
         * @return      Output string
         */
        template <typename T>
        static QString objectToString(const T& obj) noexcept;
        template <typename T>
        static QString objectToString(const tl::optional<T>& obj) noexcept;

        /**
         * @brief Deserialization template method
         *
         * @tparam T            Type of the object to be deserialized
         *
         * @param str           Input string
         * @param dummy         Used only for overload resolution (pass nullptr here)
         *
         * @return              The created element of type T
         *
         * @throws Exception if an error occurs
         */
        template <typename T>
        static T stringToObject(const QString& str, T* dummy);
        template <typename T>
        static tl::optional<T> stringToObject(const QString& str, tl::optional<T>* dummy);


    private: // Data
        Type mType;
        QString mValue; ///< either a list name, a token or a string
        QList<SExpression> mChildren;
        FilePath mFilePath;
};

/*****************************************************************************************
 *  Serialization Methods
 ****************************************************************************************/

template <>
inline QString SExpression::objectToString(const QString& obj) noexcept {
    return obj;
}

template <>
inline QString SExpression::objectToString(const char* const& obj) noexcept {
    return QString(obj);
}

template <>
inline QString SExpression::objectToString(const bool& obj) noexcept {
    return obj ? QString("true") : QString("false");
}

template <>
inline QString SExpression::objectToString(const int& obj) noexcept {
    return QString::number(obj);
}

template <>
inline QString SExpression::objectToString(const uint& obj) noexcept {
    return QString::number(obj);
}

template <>
inline QString SExpression::objectToString(const QColor& obj) noexcept {
    return obj.isValid() ? obj.name(QColor::HexArgb) : "";
}

template <>
inline QString SExpression::objectToString(const QUrl& obj) noexcept {
    return obj.isValid() ? obj.toString(QUrl::PrettyDecoded) : "";
}

template <>
inline QString SExpression::objectToString(const QDateTime& obj) noexcept {
    return obj.toUTC().toString(Qt::ISODate);
}

// all other types need to have a method "QString serializeToString() const noexcept"
template <typename T>
inline QString SExpression::objectToString(const T& obj) noexcept {
    return obj.serializeToString();
}

template <typename T>
inline QString SExpression::objectToString(const tl::optional<T>& obj) noexcept {
    if (obj) {
        return obj->serializeToString();
    } else {
        return T::sRepresentationOfNull;
    }
}

/*****************************************************************************************
 *  Deserialization Methods
 ****************************************************************************************/

template <>
inline QString SExpression::stringToObject(const QString& str, QString*) {
    return str;
}

template <>
inline bool SExpression::stringToObject(const QString& str, bool*) {
    if      (str == "true")   { return true;  }
    else if (str == "false")  { return false; }
    else throw RuntimeError(__FILE__, __LINE__, tr("Not a valid boolean."));
}

template <>
inline int SExpression::stringToObject(const QString& str, int*) {
    bool ok = false;
    int value = str.toInt(&ok);
    if (ok) { return value; }
    else throw RuntimeError(__FILE__, __LINE__, tr("Not a valid integer."));
}

template <>
inline uint SExpression::stringToObject(const QString& str, uint*) {
    bool ok = false;
    uint value = str.toUInt(&ok);
    if (ok) { return value; }
    else throw RuntimeError(__FILE__, __LINE__, tr("Not a valid unsigned integer."));
}

template <>
inline QDateTime SExpression::stringToObject(const QString& str, QDateTime*) {
    QDateTime obj = QDateTime::fromString(str, Qt::ISODate).toLocalTime();
    if (obj.isValid()) return obj;
    else throw RuntimeError(__FILE__, __LINE__, tr("Not a valid datetime."));
}

template <>
inline QColor SExpression::stringToObject(const QString& str, QColor*) {
    QColor obj(str);
    if (obj.isValid())      { return obj; }
    else throw RuntimeError(__FILE__, __LINE__, tr("Not a valid color."));
}

template <>
inline QUrl SExpression::stringToObject<QUrl>(const QString& str, QUrl*) {
    QUrl obj(str, QUrl::StrictMode);
    if (obj.isValid())      { return obj; }
    else throw RuntimeError(__FILE__, __LINE__, tr("Not a valid URL."));
}

// all other types need to have a static method "T deserializeFromString(const QString& str)"
template <typename T>
inline T SExpression::stringToObject(const QString& str, T*) {
    return T::deserializeFromString(str); // can throw
}

template <typename T>
inline tl::optional<T> SExpression::stringToObject(const QString& str, tl::optional<T>*) {
    if (str == T::sRepresentationOfNull) {
        return tl::nullopt;
    } else {
        return T::deserializeFromString(str); // can throw
    }
}

/*****************************************************************************************
 *  End of File
 ****************************************************************************************/

} // namespace librepcb

#endif // LIBREPCB_SEXPRESSION_H
