#include "dlangimagecache.h"

using namespace DlangEditor;


const QIcon &DlangIconCache::fromType(DCodeModel::SymbolType type) const {
    if (type < 0 || type >= mapping.size()) {
        throw std::runtime_error("bad symbol type");
    }
    return mapping.at(type);
}

static DlangIconCache staticIcons;

DlangIconCache &DlangIconCache::instance()
{
    return staticIcons;
}

DlangIconCache::DlangIconCache() {
    mapping.resize(DCodeModel::SymbolType::SYMBOL_IDENTIFIER_TYPE_SIZE);
    mapping[DCodeModel::SymbolType::SYMBOL_ALIAS] = QIcon(QLatin1String(":/dlangeditor/images/alias.png"));
    mapping[DCodeModel::SymbolType::SYMBOL_ARRAY] = QIcon(QLatin1String(":/dlangeditor/images/array.png"));
    mapping[DCodeModel::SymbolType::SYMBOL_ASSOC_ARRAY] = QIcon(QLatin1String(":/dlangeditor/images/assoc_array.png"));
    mapping[DCodeModel::SymbolType::SYMBOL_CLASS] = QIcon(QLatin1String(":/codemodel/images/class.png"));
    mapping[DCodeModel::SymbolType::SYMBOL_ENUM_NAME] = QIcon(QLatin1String(":/codemodel/images/enum.png"));
    mapping[DCodeModel::SymbolType::SYMBOL_ENUM_VAR] = QIcon(QLatin1String(":/codemodel/images/enumerator.png"));
    mapping[DCodeModel::SymbolType::SYMBOL_FUNCTION] = QIcon(QLatin1String(":/codemodel/images/func.png"));
    mapping[DCodeModel::SymbolType::SYMBOL_INTERFACE] = QIcon(QLatin1String(":/dlangeditor/images/interface.png"));
    mapping[DCodeModel::SymbolType::SYMBOL_KEYWORD] = QIcon(QLatin1String(":/codemodel/images/keyword.png"));
    mapping[DCodeModel::SymbolType::SYMBOL_MEMBER_VAR] = QIcon(QLatin1String(":/dlangeditor/images/member_var.png"));
    mapping[DCodeModel::SymbolType::SYMBOL_MIXIN] = QIcon(QLatin1String(":/dlangeditor/images/mixin.png"));
    mapping[DCodeModel::SymbolType::SYMBOL_MODULE] = QIcon(QLatin1String(":/codemodel/images/namespace.png"));
    mapping[DCodeModel::SymbolType::SYMBOL_PACKAGE] = QIcon(QLatin1String(":/core/images/dir.png"));
    mapping[DCodeModel::SymbolType::SYMBOL_STRUCT] = QIcon(QLatin1String(":/dlangeditor/images/struct.png"));
    mapping[DCodeModel::SymbolType::SYMBOL_TEMPLATE] = QIcon(QLatin1String(":/dlangeditor/images/template.png"));
    mapping[DCodeModel::SymbolType::SYMBOL_UNION] = QIcon(QLatin1String(":/dlangeditor/images/union.png"));
    mapping[DCodeModel::SymbolType::SYMBOL_VAR] = QIcon(QLatin1String(":/codemodel/images/var.png"));
}
