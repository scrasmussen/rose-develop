#include "sage3basic.h"
#include "UntypedJovialConverter.h"
#include "Jovial_to_ROSE_translation.h"

#define DEBUG_UNTYPED_CONVERTER 0

using namespace Untyped;
using std::cout;
using std::endl;


bool
UntypedJovialConverter::convertLabel(SgUntypedStatement* ut_stmt, SgStatement* sg_stmt,
                                     SgLabelSymbol::label_type_enum label_type, SgScopeStatement* label_scope)
{
   return UntypedJovialConverter::convertLabel(ut_stmt, sg_stmt, label_scope);
}

bool
UntypedJovialConverter::convertLabel(SgUntypedStatement* ut_stmt, SgStatement* sg_stmt, SgScopeStatement* scope)
{
   SgNode* parent;
   SgUntypedLabelStatement* ut_label_stmt;
   bool hasLabel = false;

   ROSE_ASSERT(scope != NULL);

   parent = ut_stmt->get_parent();
   if (parent != NULL)
     {
        ut_label_stmt = isSgUntypedLabelStatement(parent);
        if (ut_label_stmt != NULL)
          {
             SgLabelSymbol* label_symbol = NULL;
             SgLabelStatement* label_stmt = NULL;

          // This statement has a label get the SgLabelStatement from the scope and insert it
             SgName label_name(ut_label_stmt->get_label_string());

             SgFunctionDefinition * label_scope = SageInterface::getEnclosingFunctionDefinition(scope, true);
             ROSE_ASSERT (label_scope);

             label_symbol = label_scope->lookup_label_symbol(label_name);
             ROSE_ASSERT(label_symbol != NULL);

             label_stmt = label_symbol->get_declaration();
             ROSE_ASSERT(label_stmt != NULL);

             label_stmt->set_statement(sg_stmt);
             sg_stmt->set_parent(label_stmt);

             hasLabel = true;
          }
     }

   return hasLabel;
}

SgStatement*
UntypedJovialConverter::convertSgUntypedCaseStatement (SgUntypedCaseStatement* ut_stmt, SgNodePtrList& children, SgScopeStatement* scope)
  {
     SgStatement* sg_stmt = UntypedConverter::convertSgUntypedCaseStatement(ut_stmt, children, scope);

  // If a Jovial CaseAlternative rule doesn't have a FALLTHRU, then create a
  // compiler-generated break statement so that program analysis will be the same as for C.
  //
     if (ut_stmt->get_has_fall_through() == false && isSgSwitchStatement(sg_stmt) == false)
       {
          SgBreakStmt* sg_break_stmt = SageBuilder::buildBreakStmt();
          ROSE_ASSERT(sg_break_stmt);

          sg_break_stmt->setCompilerGenerated();
          SageInterface::appendStatement(sg_break_stmt, scope);
       }

     return sg_stmt;
  }
