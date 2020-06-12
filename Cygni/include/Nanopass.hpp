#ifndef CYGNI_NANOPASS
#define CYGNI_NANOPASS
#include "Expression.hpp"

namespace cygni
{
	class Nanopass
	{
	public:
	};

	class TypeRenamer
	{
	public:
		void RenameAll(Project& project);
		void RenameMethod(MethodDef& method, Table<std::u32string, TypeAlias>& typeAliases);
		void RenameField(FieldDef& field, Table<std::u32string, TypeAlias>& typeAliases);
		TypePtr RenameType(TypePtr type, Table<std::u32string, TypeAlias>& typeAliases);
	};

	class InheritanceTypeResolver
	{
	public:
		// resolve the super types of classes
		void VisitProject(Project& project);
		void VisitClass(Project& project, std::shared_ptr<ClassInfo> classInfo);
	};

	class InheritanceProcessor
	{
	public:
		void VisitProject(Project& project);
		void VisitClass(Project& project, std::shared_ptr<ClassInfo> classInfo);
	};

} // namespace cygni
#endif // CYGNI_NANOPASS