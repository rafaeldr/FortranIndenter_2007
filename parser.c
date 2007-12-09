/*
 *	TRABALHO DE COMPILADORES: IDENTADOR DE CÓDIGO FORTRAN 77
 *
 *	ALUNOS:
 *	RAFAEL DELALIBERA RODRIGUES
 *	PAULO HENRIQUE ARANTES URGAL
 *
 *  Arquivo: Parser.c
 *
 */

#include "defaults.h"
#include "keywords.h"
#include "parser.h"

/*
 *       Lexer Members:
 */
 
extern token_t lookahead;
extern void match (token_t);
extern char lexeme[];

/* 
 *     Recursive Top-Down Parser Analyser (For Most of Productions)
 */

void ProgramUnit() {
     switch (lookahead) {
            case PROGRAM:
                 MainProgram();
                 break;
            case FUNCTION:
                 FunctionSubprogram();
                 break;
            case SUBROUTINE:
                 SubroutineSubprogram();
                 break;
            case BLOCKDATA:
                 BlockDataSubprogram();
                 break;
            default:
                    MainProgram(); 
                    break;
     }
}
     
void MainProgram() {
     if(lookahead == PROGRAM){
                  ProgramStmt();
                  MainRange();
     } 
     else MainRange();
}

void MainRange() {
     if (lookahead == END){
                   EndProgramStmt();
     }
     else {
          Body();
          EndProgramStmt();
     }
}

void EndProgramStmt() {
     match(END);
     EOS();
}

void Body() {
     BodyConstruct();
     while (lookahead != END) BodyConstruct();
}

void BodyConstruct() {
     switch (lookahead){
            /* SpecificationPartConstruct */
            case IMPLICIT:
            case PARAMETER:
            case FORMAT:
            case ENTRY:
            /* SpecificationPartConstruct:DeclarationConstruct:TypeDeclaration */
            case INTEGER:
            case REAL:
            case DOUBLEPRECISION:
            case COMPLEX:
            case LOGICAL:
            case CHARACTER:               
            /* SpecificationPartConstruct:DeclarationConstruct:SpecificationStmt */     
            case COMMON:
            case DATA:
            case DIMENSION:
            case EQUIVALENCE:
            case EXTERNAL:
            case INTRINSIC:
            case SAVE:
                 SpecificationPartConstruct();     
                 break;
            default:
                    ExecutableConstruct();
                    break;
     }
}

void SpecificationPartConstruct() {
     switch (lookahead){
            case IMPLICIT:
                 ImplicitStmt();
                 break;
            case PARAMETER:
                 ParameterStmt();
                 break;
            case FORMAT:
                 FormatStmt();
                 break;
            case ENTRY:
                 EntryStmt();
                 break;
            default:
                    DeclarationConstruct();
                    break;
     }
}

void DeclarationConstruct(){
     switch (lookahead){
            case INTEGER:
            case REAL:
            case DOUBLEPRECISION:
            case COMPLEX:
            case LOGICAL:
            case CHARACTER:
                 TypeDeclarationStmt();
                 break;
            default:
                    SpecificationStmt();
                    break;
     }
}

void FunctionSubprogram() {
     FunctionPrefix();
     FunctionName();
     FunctionRange();
}

void FunctionRange() {
     FunctionParList();
     EOS();
     if (lookahead == END){
                   EndFunctionStmt();
     }
     else {
          Body();
          EndFunctionStmt();
     }
}

void EndFunctionStmt() {
     match(END);
     EOS();
}

void SubroutineSubprogram() {
     match(SUBROUTINE);
     SubroutineName();
     SubroutineRange();
}

void SubroutineRange() {
     SubroutineParList();
     EOS();
     if (lookahead == END){
                   EndSubroutineStmt();
     }
     else {
          Body();
          EndSubroutineStmt();
     }
}

void EndSubroutineStmt() {
     match(END);
     EOS();
}

void BlockDataSubprogram() {
     BlockDataStmt();
     if (lookahead == END){
                   EndBlockDataStmt();
     }
     else {
          BlockDataBody();
          EndBlockDataStmt();
     }
}

void EndBlockDataStmt() {
     match(END);
     EOS();
}

void BlockDataBody() {
     BlockDataBodyConstruct();
     while (lookahead != END) BlockDataBodyConstruct();
}

void BlockDataBodyConstruct() {
     SpecificationPartConstruct();
}

void SpecificationStmt() {
     switch (lookahead) {
            case COMMON:
                 CommonStmt();
                 break;
            case DIMENSION:
                 DimensionStmt();
                 break;
            case EQUIVALENCE:
                 EquivalenceStmt();
                 break;
            case EXTERNAL:
                 ExternalStmt();
                 break;
            case INTRINSIC:
                 IntrinsicStmt();
                 break;
            case SAVE:
                 SaveStmt();
                 break;
     }
}

void ExecutionPartConstruct() {
     switch (lookahead){
            case FORMAT:
                 FormatStmt();
                 break;
            case DATA:
                 DataStmt();
                 break;
            case ENTRY:
                 EntryStmt();
                 break;
            default:
                    ExecutableConstruct();
                    break;
     }
}
     
void ExecutableConstruct(){
     switch (lookahead){
            case DO:
                 DoConstruct();
                 break;
            case IF:
                 IfConstruct();
                 break;
            default:
                    ActionStmt();
                    break;
     }
}

/*
 *    This Procedure Must Be Revised
 */
void ActionStmt() {
     switch (lookahead){
            case ASSIGN:
                 AssignStmt();
                 break;
            case BACKSPACE:
                 BackspaceStmt();
                 break;
            case CALL:
                 CallStmt();
                 break;
            case CLOSE:
                 CloseStmt();
                 break;
            case CONTINUE:
                 ContinueStmt();
                 break;
            case ENDFILE:
                 EndfileStmt();
                 break;
            case INQUIRE:
                 InquireStmt();
                 break;
            case OPEN:
                 OpenStmt();
                 break;
            case PAUSE:
                 PauseStmt();
                 break;
            case PRINT:
                 PrintStmt();
                 break;
            case READ:
                 ReadStmt();
                 break;
            case RETURN:
                 ReturnStmt();
                 break;
            case REWIND:
                 RewindStmt();
                 break;
            case STOP:
                 StopStmt();
                 break;
            case WRITE:
                 WriteStmt();
                 break;
     }
}

void ProgramStmt(){
     match(PROGRAM);
     ProgramName();
     EOS();
}

void EntryStmt(){
     match(ENTRY);
     EntryName();
     SubroutineParList();
     EOS();
}

void FunctionStmt(){
     FunctionPrefix();
     Name();
     FunctionParList();
     EOS();
}

void FunctionPrefix(){
     if (lookahead == FUNCTION){
                   match(FUNCTION);
     }
     else{
          TypeSpec();
          match(FUNCTION);
     }
}

void FunctionParList(){
     if (lookahead == '('){
                   match('(');
                   FunctionPars();
                   match(')');
     }
}

void FunctionPars(){
     if (lookahead == ID){
        FunctionPar();
        while (lookahead != ','){
              match(',');
              FunctionPar();
        }
     }
}

void FunctionPar(){
     DummyArgName();
}

void SubroutineStmt(){
     match(SUBROUTINE);
     Name();
     SubroutineParList();
     EOS();
}

void SubroutineParList(){
     if (lookahead == '('){
                   match('(');
                   SubroutinePars();
                   match(')');
     }
}

void SubroutinePars(){
     if (lookahead == ID){
        SubroutinePar();
        while (lookahead != ','){
              match(',');
              SubroutinePar();
        }
     }
}     

void SubroutinePar(){
     if (lookahead == '*'){
                   match('*');
     }     
     else {
          DummyArgName();
     }
}

void BlockDataStmt(){
     match(BLOCKDATA);
     if (lookahead == ID) {
           BlockDataName();
     }
     EOS();
}

void DimensionStmt(){
     match(DIMENSION);
     ArrayDeclaratorList();
     EOS();
}

void ArrayDeclaratorList(){
     ArrayDeclarator();
     if (lookahead == ','){
                   match(',');
                   ArrayDeclaratorList();
     }
}
     
void ArrayDeclarator(){
     VariableName();
     match('(');
     ArraySpec();
     match(')');
}
     
/*
 *   Procedure Under Construction
 *
void ArraySpec();
 */

void ExplicitShapeSpecList(){
     ExplicitShapeSpec();
     while (lookahead == ','){
                   match(',');
                   ExplicitShapeSpec();
     }
}

/*
 *   Procedure Under Construction
 *
void ExplicitShapeSpec(){
 */

void LowerBound(){
     Expr();
}
     
void UpperBound(){
     Expr();
}

/*
 *   Procedure Under Construction
 *
void AssumedSizeSpec(){
     if (lookahead == '*'){
                   match('*');
     }
     else{
 */

void EquivalenceStmt(){
     match(EQUIVALENCE);
     EquivalenceSetList();
     EOS();
}

void EquivalenceSetList(){
     EquivalenceSet();
     if (lookahead == ','){
                   match(',');
                   EquivalenceSetList();
     }
}

void EquivalenceSet(){
     match('(');
     EquivalenceObject();
     match(',');
     EquivalenceObjectList();
     match(')');
}

void EquivalenceObjectList(){
     EquivalenceObject();
     if (lookahead == ','){
                   match(',');
                   EquivalenceObjectList();
     }
}

void EquivalenceObject(){
     Variable();
}

void CommonStmt(){
     match(COMMON);
     Comlist();
     EOS();
}

/* 
 *   This Procedure Must Be Revised
 */
void Comlist(){
     switch (lookahead){
            case ID:
                 CommonBlockObject();
                 break;
            case '/':
                 Comblock();
                 CommonBlockObject();
                 break;
     }
     while (lookahead == ','){
           match(',');
           switch (lookahead){
                  case ID:
                       CommonBlockObject();
                       break;
                  case '/':
                       Comblock();
                       CommonBlockObject();
                       break;
           }
     }
}
     
void Comblock(){
     match('/');
     if (lookahead == '/'){
                   match('/');
     }
     else{
          CommonBlockName();
          match('/');
     }
}

void CommonBlockObject(){
     VariableName();
     if (lookahead == '('){
                   match('(');
                   ArraySpec();
                   match(')');
     }
}

void TypeDeclarationStmt(){
     TypeSpec();
     EntityDeclList();
     EOS();
}

void TypeSpec(){
     switch (lookahead){
            case INTEGER:
                 match(INTEGER);
                 break;
            case REAL:
                 match(REAL);
                 break;
            case DOUBLEPRECISION:
                 match(DOUBLEPRECISION);
                 break;            
            case COMPLEX:
                 match(COMPLEX);
                 break;
            case LOGICAL:
                 match(LOGICAL);
                 break;
            case CHARACTER:
                 if (lookahead == '*'){
                               match(CHARACTER);
                               LengthSelector();
                 }
                 else{
                      match(CHARACTER);
                 }
                 break;
            default:
                    match(ERROR);
                    break;
     }
}

void EntityDeclList(){
     EntityDecl();
     if (lookahead == ','){
                   match(',');
                   EntityDeclList();
     }
}
     
void EntityDecl(){
     ObjectName();
     switch (lookahead){
            case '(':
                 match('(');
                 ArraySpec();
                 match(')');
                 if (lookahead == '*'){
                               match('*');
                               CharLenght();
                 }
                 break;
            case '*':
                 match('*');
                 CharLength();
                 break;
            default:
                    match(ERROR);
                    break;
     }
}

void LengthSelector(){
     match('*');
     CharLength();
}

void ImplicitStmt(){
     match(IMPLICIT);
     ImplicitSpecList();
     EOS();
}

void ImplicitSpecList(){
     ImplicitSpec();
     if (lookahead == ','){
                   match(',');
                   ImplicitSpecList();
     }
}

void ImplicitSpec(){
     TypeSpec();
     Impl();
}

/*
 *   Procedure Under Construction
 *
void Impl();
 */

void CharLength(){
     if (lookahead == '('){
                   match('(');
                   TypeParamValue();
                   match(')');
     }
     else Icon();
}

void TypeParamValue(){
     if (lookahead == '*'){
                   match('*');
     }
     else Expr();
}

void ParameterStmt(){
     match(PARAMETER);
     match('(');
     NamedConstantDefList();
     match(')');
     EOS();
}

void NamedConstantDefList(){
     NamedConstantDef();
     if (lookahead == ','){
                   match(',');
                   NamedConstantDefList();
     }
}

void NamedConstantDef(){
     NamedConstant();
     match('=');
     Expr();
}

void NamedConstant(){
     Ident();
}

void NamedConstantUse(){
     Ident();
}

void ExternalStmt(){
     match(EXTERNAL);
     ExternalNameList();
     EOS();
}

void ExternalNameList(){
     ExternalName();
     if (lookahead == ','){
                   match(',');
                   ExternalNameList();
     }
}

void IntrinsicStmt(){
     match(INTRINSIC);
     IntrinsicList();
     EOS();
}

void IntrinsicList(){
     IntrinsicProcedureName();
     if (lookahead == ','){
                   match(',');
                   IntrinsicList();
     }
}

void SaveStmt(){
     match(SAVE);
     if (lookahead == ID || lookahead == '/'){
                   SavedEntityList();
     }
     EOS();
}

void SavedEntityList(){
     SavedEntity();
     if (lookahead == ','){
                   match(',');
                   SavedEntityList();
     }
}

void SavedEntity(){
     if (lookahead == '/'){
                   SavedCommonBlock();
     }
     else VariableName();
}

void SavedCommonBlock(){
     match('/');
     CommonBlockName();
     match('/');
}

void DataStmt(){
     match(DATA);
     DataList();
     EOS();
}

void Datalist(){
     DataStmtSet();
     if (lookahead == ','){
                   match(',');
                   Datalist();
     }
     if (lookahead == '('){
                   match('(');
                   Datalist();
     }
     if (lookahead == ID){
                   match(ID);
                   Datalist();
     }
}
     
void DataStmtSet(){
     DataStmtObjectList();
     match('/');
     DataStmtValueList();
     match('/');
}

void DataStmtObjectList(){
     DataStmtObject();
     if (lookahead == ','){
                   match(',');
                   DataStmtObjectList();
     }
}

void DataStmtObject(){
     if (lookahead == '('){
                   DataImpliedDo();
     }
     else Variable();
}

void DataStmtValueList(){
     DataStmtValue();
     if (lookahead == ','){
                   match(',');
                   DataStmtValueList();
     }
}

void DataStmtValue(){
     if (lookahead == ICON){
                   match(ICON);
                   match('*');
                   Constant();
     }
     else{
          if (lookahead == ID){
                        NamedConstantUse();
                        match('*');
                        Constant();
          }
          else Constant();
     }
}

void DataImpliedDo(){
     match('(');
     DataIDoObjectList();
     match(',');
     ImpliedDoVariable();
     match('=');
     Expr();
     match(',');
     Expr();
     if (lookahead == ','){
                   match(',');
                   Expr();
                   match(')');
     }
     else match(')');
}

void DataIDoObjectList(){
     DataIDoObject();
     if (lookahead == ','){
                   match(',');
                   DataIDoObjectList();
     }
}

void DataIDoObject(){
     if (lookahead == '('){
                   DataImpliedDo();
     }
     else ArrayElement();
}

void AssignmentStmt(){
     Name();
     switch (lookahead){
            case '=':
                 match('=');
                 Expr();
                 EOS();
                 break;
            case '(':
                 match('(');
                 SFExprList();
                 match(')');
                 if (lookahead == '='){
                               match('=');
                               Expr();
                               EOS();
                 }
                 else {
                      SubstringRange();
                      match('=');
                      Expr();
                      EOS();
                 }
                 break;
            default:
                    match(ERROR);
                    break;
     }
}

/*     
 *     Procedure Under Construction
 *
void SFExprList(){
     if (lookahead == ':'){
                   match(':');
                   if (lookahead != ')'){
                                 Expr();
                   }
     }
     else {
*/

void GotoStmt(){
     match(GOTOKW);
     LblRef();
     EOS();
}

void ComputedGotoStmt(){
     match(GOTOKW);
     match('(');
     LblRefList();
     match(')');
     if (lookahead == ','){
                   CommaExp();
                   EOS();
     }
     else {
          Expr();
          EOS();
     }
}

void CommaExp(){
     match(',');
     Expr();
}

void LblRefList(){
     LblRef();
     if (lookahead == ','){
                   match(',');
                   LblRefList();
     }
}

void LblRef(){
     match(LABEL);
}

void AssignedGotoStmt(){
     match(GOTOKW);
     VariableName();
     if (lookahead == '('){
                   match('(');
                   LblRefList();
                   match(')');
                   EOS();
     }
     else {
          if (lookahead == ','){
                        match(',');
                        match('(');
                        LblRefList();
                        match(')');
                        EOS();
          }
          else EOS();
     }
}

void VariableComma(){
     VariableName();
     match(',');
}

void ArithmeticIfStmt(){
     match(IF);
     match('(');
     Expr();
     match(')');
     ActionStmt();
}

void IfConstruct(){
     IfThenStmt();
     ThenPart();
}

/*
 *   Procedure Under Construction
 *
void ThenPart(){
  */
  
void ElseIfConstruct(){
     ElseIfStmt();
     ThenPart();
}

void ElseConstruct(){
     ElseStmt();
     ElsePart();
}

void ElsePart(){
     if (lookahead == ENDIF){
                   EndIfStmt();
     }
     else {
          ConditionalBody();
          EndIfStmt();
     }
}

void ConditionalBody(){
     ExecutionPartConstruct();
     if (lookahead != ENDIF || lookahead != ELSE || lookahead != ELSEIF){
                   ConditionalBody();
     }
}

void IfThenStmt(){
     match(IF);
     match('(');
     Expr();
     match(')');
     match(THEN);
     EOS();
}

void ElseIfStmt(){
     match(ELSEIF);
     match('(');
     Expr();
     match(')');
     match(THEN);
     EOS();
}

void ElseStmt(){
     match(ELSE);
     EOS();
}

void EndIfStmt(){
     match(ENDIF);
     EOS();
}

void DoConstruct(){
     LabelDoStmt();
}

void LabelDoStmt(){
     match(DO);
     LblRef();
     CommaLoopControl();
     EOS();
}

void CommaLoopControl(){
     if (lookahead == ','){
                   match(',');
                   LoopControl();
     }
     else LoopControl();
}

void LoopControl(){
     VariableName();
     match('=');
     Expr();
     match(',');
     Expr();
     if (lookahead == ','){
                   match(',');
                   Expr();
     }
}
     
void ContinueStmt(){
     match(CONTINUE);
     EOS();
}

void StopStmt(){
     match(STOP);
     if (lookahead == ICON){
                   Icon();
                   EOS();
     }
     else {
          if (lookahead == SCON){
                        Scon();
                        EOS();
          }
          else EOS();
     }
}

void PauseStmt(){
     match(PAUSE);
     if (lookahead == ICON){
                   Icon();
                   EOS();
     }
     else {
          if (lookahead == SCON){
                        Scon();
                        EOS();
          }
          else EOS();
     }
}

void WriteStmt(){
     match(WRITE);
     match('(');
     IoControlSpecList();
     match(')');
     if (lookahead == EOD){
                   EOS();
     }
     else {
          OutputItemList();
          EOS();
     }
}

void ReadStmt(){
     match(READ);
     switch (lookahead){
            case '(':
                 RdCtlSpec();
                 if (lookahead == EOD){
                               EOS();
                 }
                 else {
                      InputItemList();
                      EOS();
                 }
                 break;
            default:
                    RdFmtId();
                    if (lookahead == EOD){
                                  EOS();
                    }
                    else {
                         InputItemList();
                         EOS();
                    }
     }
}

/*
 * Procedure Under Construction
 *
void RdCtlSpec();
 */
     
void RdUnitId(){
     match('(');
     if (lookahead == '*'){
                   match('*');
                   match(')');
     }
     else {
          UFExpr();
          match(')');
     }
}

void PrintStmt(){
     match(PRINT);
     FormatIdentifier();
     if (lookahead == ','){
                   match(',');
                   OutputItemList();
                   EOS();
     }
     else EOS();
}

void IoControlSpecList(){
     switch (lookahead){
           case 'fmt=':
           case 'unit=':
           case 'rec=':
           case 'end=':
           case 'err=':
           case 'iostat=':
                IoControlSpec();
                while (lookahead == ',') {
                      match(',');
                      IoControlSpec();
                }
                break;
           default:
                   UnitIdentifier();
                   match(',');
                   if (lookahead == '('  ||
                       lookahead == '*'  ||
                       lookahead == SCON ||
                       lookahead == ID){
                                 FormatIdentifier();
                   }
                   else {
                        if (lookahead == 'fmt='   ||
                            lookahead == 'unit='  ||
                            lookahead == 'rec='   ||
                            lookahead == 'end='   ||
                            lookahead == 'err='   ||
                            lookahead == 'iostat='){
                                      IoControlSpec();
                        }
                   }
                   break;
     }
}
                   
void IoControlSpec(){
     switch (lookahead){
            case 'fmt=':
                 match('fmt=');
                 FormatIdentifier();
                 break;
            case 'unit=':
                 match('unit=');
                 UnitIdentifier();
                 break;
            case 'rec=':
                 match('rec=');
                 Expr();
                 break;
            case 'end=':
                 match('end=');
                 LblRef();
                 break;
            case 'err=':
                 match('err=');
                 LblRef();
                 break;
            case 'iostat=':
                 match('iostat=');
                 ScalarVariable();
                 break;
            default:
                    match(ERROR);
                    break;
     }
}

void InputItemList(){
     InputItem();
     if (lookahead == ','){
                   match(',');
                   InputItemList();
     }
}

void InputItem(){
     if (lookahead == '('){
                   InputImpliedDo();
     }
     else {
          Variable();
     }
}

/*
 * Procedure Under Construction
 *
void OutputItemList();
 */
 
void InputImpliedDo(){
     match('(');
     InputItemList();
     match(',');
     ImpliedDoVariable();
     match('=');
     Expr();
     match(',');
     Expr();
     if (lookahead == ','){
                   match(',');
                   Expr();
                   match(')');
     }
     else match(')');
}

/*
 * Procedure Under Construction
 *
void OutputImpliedDo(){
     match('(');
 */
 
void OpenStmt(){
     match(OPEN);
     match('(');
     ConnectSpecList();
     match(')');
     EOS();
}

void ConnectSpecList(){
     if (lookahead == 'unit='   ||
         lookahead == 'err='    ||
         lookahead == 'file='   ||
         lookahead == 'status=' ||
         lookahead == 'access=' ||
         lookahead == 'form='   ||
         lookahead == 'recl='   ||
         lookahead == 'blank='  ||
         lookahead == 'iostat=' ){
                   ConnectSpec();
     }
     else UnitIdentifier();
     if (lookahead == ','){
                   match(',');
                   ConnectSpecList();
     }
}

void ConnectSpec(){
     switch (lookahead){
            case 'unit=':
                 match('unit=');
                 UnitIdentifier();
                 break;
            case 'err=':
                 match('err=');
                 LblRef();
                 break;
            case 'file=':
                 match('file=');
                 CExpr();
                 break;
            case 'status=':
                 match('status=');
                 CExpr();
                 break;
            case 'access:':
                 match('access:');
                 CExpr();
                 break;
            case 'form=':
                 match('form=');
                 CExpr();
                 break;
            case 'recl=':
                 match('recl=');
                 CExpr();
                 break;
            case 'blank=':
                 match('blank=');
                 CExpr();
                 break;
            case 'iostat=':
                 match('iostat=');                 
                 ScalarVariable();
                 break;
            default:
                    match(ERROR);
                    break;
     }
}

void CloseStmt(){
     match(CLOSE);
     match('(');
     CloseSpecList();
     match(')');
     EOS();
}

void CloseSpecList(){
     if (lookahead == 'unit='  ||
         lookahead == 'err='   ||
         lookahead == 'status='||
         lookahead == 'iostat='){
                   CloseSpec();
     }
     else UnitIdentifier();
     if (lookahead == ','){
                   match(',');
                   CloseSpecList();
     }
}

void CloseSpec(){
     switch (lookahead){
            case 'unit=':
                 match('unit');
                 UnitIdentifier();
                 break;
            case 'err=':
                 match('err=');
                 LblRef();
                 break;
            case 'status=':
                 match('status=');
                 CExpr();
                 break;
            case 'iostat=':
                 match('iostat=');
                 ScalarVariable();
                 break;
            default:
                    match(ERROR);
                    break;
     }
}

void InquireStmt(){
     match(INQUIRE);
     match('(');
     InquireSpecList();
     match(')');
     EOS();
}

void InquireSpecList(){
     if (lookahead == 'unit='       ||
         lookahead == 'file='       ||
         lookahead == 'err='        ||
         lookahead == 'iostat='     ||
         lookahead == 'exist='      ||
         lookahead == 'opened='     ||
         lookahead == 'number='     ||
         lookahead == 'named='      ||
         lookahead == 'name='       ||
         lookahead == 'access='     ||
         lookahead == 'sequential=' ||
         lookahead == 'direct='     ||
         lookahead == 'form='       ||
         lookahead == 'formatted='  ||
         lookahead == 'unformatted='||
         lookahead == 'recl='       ||
         lookahead == 'nectrec='    ||
         lookahead == 'blank='      ){
                   InquireSpec();
        }
        else UnitIdentifier();
        if (lookahead == ','){
                      match(',');
                      InquireSpecList();
        }
}

void InquireSpec(){
     switch (lookahead){
            case 'unit=':
                 match('unit=');
                 UnitIdentifier();
                 break;
            case 'file=':
                 match('file=');
                 CExpr();
                 break;
            case 'err=':
                 match('err=');
                 LblRef();
                 break;
            case 'iostat=':
                 match('iostat=');
                 ScalarVariable();
                 break;
            case 'exist=':
                 match('exist=');
                 ScalarVariable();
                 break;
            case 'opened=':
                 match('opened=');
                 ScalarVariable();
                 break;
            case 'number=':
                 match('number=');
                 ScalarVariable();
                 break;
            case 'named=':
                 match('named=');
                 ScalarVariable();
                 break;
            case 'name=':
                 match('name=');
                 ScalarVariable();
                 break;
            case 'access=':
                 match('access=');
                 ScalarVariable();
                 break;
            case 'sequential=':
                 match('sequential=');
                 ScalarVariable();
                 break;
            case 'direct=':
                 match('direct=');
                 SclarVariable();
                 break;
            case 'form=':
                 match('form=');
                 ScalarVariable();
                 break;
            case 'formatted=':
                 match('formatted=');
                 ScalarVariable();
                 break;
            case 'unformatted=':
                 match('unformatted=');
                 ScalarVariable();
                 break;
            case 'recl=':
                 match('recl=');
                 Expr();
                 break;
            case 'nextrec=':
                 match('nextrec=');
                 ScalarVariable();
                 break;
            case 'blank=':
                 match('blank=');
                 ScalarVariable();
                 break;
            default:
                    match(ERROR);
                    break;
     }
}
                   
void BackspaceStmt(){
     match(BACKSPACE);
     if (lookahead == '('){
                   match('(');
                   PositionSpecList();
                   match(')');
                   EOS();
     }
     else {
          UnitIdentifier();
          EOS();
     }
}

void EndFileStmt(){
     match(ENDFILE);
     if (lookahead == '('){
                   match('(');
                   PositionSpecList();
                   match(')');
                   EOS();
     }
     else {
          UnitIdentifier();
          EOS();
     }
}

void RewindStmt(){
     match(REWIND);
     if (lookahead == '('){
                   match('(');
                   PositionSpecList();
                   match(')');
                   EOS();
     }
     else {
          UnitIdentifier();
          EOS();
     }
}

void PositionSpecList(){
     if (lookahead == 'unit='  ||
         lookahead == 'err='   ||
         lookahead == 'iostat='){
                   PositionSpec();
     }
     else {
          UnitIdentifier;
          match(',');
          PositionSpec();
     }
     if (lookahead == ','){
                   match(',');
                   PositionSpecList();
     }
}

void PositonSpec(){
     switch (lookahead){
            case 'unit=':
                 match('unit=');
                 UnitIdentifier();
                 break;
            case 'err=':
                 match('err=');
                 LblRef();
                 break;
            case 'iostat=':
                 match('iostat=');
                 ScalarVariable();
                 break;
            default:
                    match(ERROR);
                    break;
     }
}

void UnitIdentifier(){
     if (lookahead == '*'){
                  match('*');
     }
     else UFExpr();
}

void FormatIdentifier(){
     if (lookahead == '*'){
                   match('*');
     }
     else {
          if (lookahead == LABEL){
                        LblRef();
          }
          else CExpr();
     }
}

void FormatStmt(){
     match(FORMAT);
     match('(');
     FmtSpec();
     match(')');
     EOS();
}

/* FORMAT SPEC TREATMENT WILL NOT BE DEALED */

void StmtFunctionStmt(){
     Name();
     StmtFunctionRange();
}

void StmtFunctionRange(){
     match('(');
     if (lookahead == ')'){
                   match(')');
                   match('=');
                   Expr();
                   EOS();
     }
     else {
          SFDummyArgNameList();
          match(')');
          match('=');
          Expr();
          EOS();
     }
}

void SFDummyArgNameList(){
     SFDummyArgName();
     if (lookahead == ','){
                   match(',');
                   SFDummyArgNameList();
     }
}

void CallStmt(){
     match(CALL);
     SubroutineNameUse();
     if (lookahead == '('){
                   match('(');
                   SubroutineArgList();
                   match(')');
                   EOS();
     }
     else EOS();
}

/*
 *   Procedure Under Construction
 *
void SubroutineArgList(){
 */
 
void SubroutineArg(){
     switch (lookahead){
            case '*':
                 match('*');
                 LblRef();
                 break;
            case HCON:
                 Hcon();
                 break;
            default:
                    Expr();
                    break;
     }
}

void FunctionReference(){
     Name();
     match('(');
     match(')');
}

void ComplexDataRef1(){
     Name();
     match('(');
     SectionSubscriptList();
     match(')');
}

void ComplexDataRef(){
     ComplexDataRef1();
     while (lookahead == ID) ComplexDataRef1();
     match('(');
     SectionSubscriptList();
     match(')');
}

void SectionSubscriptList(){
     SectionSubscript();
     if (lookahead == ','){
                   match(',');
                   SectionSubscriptList();
     }
}

/*
 *   Procedure Under Construction
 *
void SectionSubscript(){
     S     
 */   
 
void Expr(){
     Level5Expr();
}

/*
 *   Procedure Under Construction
 *
void Primary(){
 */
 
void Level1Expr(){
     Primary();
}

void MultOperand(){
     Level1Expr();
     if (lookahead == '**'){
                   PowerOp();
                   MultOperand();
     }
}

void AddOperand(){
     if (lookahead == '*' || lookahead == '/'){
                   MultOp();
                   AddOperand();
     }
     else MultiOperand();
}

/*                 
 *    Procedure Under Construction
 *
void Level2Expr(){
     if (lookahead == '+' || lookahead == '-'){
                   AddOp();
                   Level2Expr();
     }
     else {
          if 
          AddOperand();
 */     

void PowerOp(){
     match('**');
}

void MultiOp(){
     switch (lookahead){
            case '*':
                 match('*');
                 break;
            default:
                 match('/');
                 break;
     }
}

void AddOp(){
     switch (lookahead){
            case '-':
                 match('-');
                 break;
            default:
                    match('+');
                    break;
     }
}

void Sign(){
     switch (lookahead){
            case '-':
                 match('-');
                 break;
            default:
                    match('+');
                    break;
     }
}

void UFExpr(){
     if (lookahead == '+' || lookahead == '-'){
                   Sign();
                   UFterm();
     }
     else UFTerm;
}

void UFTerm(){
     if (lookahead == ICON ||
         lookahead == SCON ||
         lookahead == ID   ||
         lookahead == '('  ){
                   UFFactor();
     }
     else {
          while (lookahead == ICON ||
                 lookahead == SCON ||
                 lookahead == ID   ||
                 lookahead == '('  ){
                           UFFactor();
                           
          }
          if (lookahead == '*' || lookahead == '/'){
                        MultiOp();
                        UFFactor();
          }
          else {
               ConcatOp();
               UFPrimary();
          }
     }
}     

void UFFactor(){
     UFPrimary();
     if (lookahead == '**'){
                   PowerOp();
                   UFFactor();
     }
}

/*
 *   Procedure Under Construction
 *
void UFPrimary(){
     switch (lookahead){
            case ICON:
                 Icon();
                 break;
            case SCON:
                 Scon();
                 break;
            case ID:
*/

/*
 *  Procedure Under Construction
 * 
void CExpr();
*/

void CPrimary(){
     COperand();
     if (lookahead == '('){
                   match('(');
                   CExpr();
                   match(')');
     }
}

/*
 *   Procedure Under Construction
 *
void COperand(){
     switch (lookahead){
            case SCON:
                 Scon();
                 break;
            case ID
*/
                 
/*                 
 *    Procedure Under Construction
 *
void Primary(){
     Scon();
}*/

void Level3Expr(){
     
void ConcatOp(){
     match('//');
}

void AndOperand(){
     if (lookahead == '.not.'){
                   NotOp();
     }
     else Level4Expr();
}

void OrOperand(){
     if (lookahead == '.and.'){
                   while (lookahead == '.and.'){
                         AndOperand();
                   }
                   AndOp();
     }
     else AndOperand();
}

/*
 *   Procedure Under Construction
 *
void EquivOperand(){
     if (lookahead == '                    
*/     

/*
 *     Procedure Under Construction
 *
void Level5Expr(){
*/

void NotOp(){
     match('.not.');
}

void AndOp(){
     match('.and.');
}

void OrOp(){
     match('.or.');
}

void EquivOp(){
     switch (lookahead){
            case '.eqv.':
                 match('.eqv.');
                 break;
            default:
                    match('.neqv.');
                    break;
     }
}

void RelOp(){
     switch (lookahead){
            case '.eq.':
                 match('.eq.');
                 break;
            case '.ne.':
                 match('.ne.');
                 break;
            case '.lt.':
                 match('.lt.');
                 break;
            case '.le.':
                 match('.le.');
                 break;
            case '.gt.':
                 match('.gt.');
                 break;
            default:
                    match('.ge.');
                    break;
     }
}

void ArrayElement(){
     VariableName();
     match('(');
     SectionSubscriptList();
     match(')');
}

void SubstringRange(){
     match('(');
     SubscriptTriplet();
     match(')');
}

/*
 *   Procedure Under Construction
 *
void SubscriptTriplet(){
     if (lookahead == ':'){
                   match(':');
 */

void Name(){
     match(ID);
}

void Constant(){
     switch (lookahead){
            case ID:
                 NamedConstantUse();
                 break;
            case '+':
                 match('+');
                 UnsignedArithmeticConstant();
                 break;
            case '-':
                 match('-');
                 UnsignedArithmeticConstant();
            case '.true.':
                 LogicalConstant();
            case '.false.':
                 LogicalConstant();
            case SCON:
                 Scon();
                 break;
            case HCON:
                 Hcon();
                 break;
            case ICON:
                 UnsignedArithmeticConstant();
                 break;
            case DCON:
                 UnsignedArithmeticConstant();
                 break;
            case RCON:
                 UnsignedArithmeticConstant();
                 break;
            case '(':
                 UnsignedArithmeticConstant();
                 break;
            default:
                    match(ERROR);
                    break;
     }                   
}
     
void UnsignedArithmeticConstant(){
     switch (lookahead){
            case ICON:
                 Icon();
                 break;
            case RCON:
                 Rcon();
                 break;
            case DCON:
                 Dcon();
                 break;
            default:
                 ComplexConst();
                 break;
     }
}

void ComplexConstant(){
     match('(');
     Expr();
     match(',');
     Expr();
     match(')');
}

void LogicalConstant(){
     switch (lookahead){
            case '.true.':
                 match('.true.');
                 break;
            default:
                    match('.false.');
                    break;
     }
}

void Label(){
     Icon();
}

/*
 *   Nonterminal that Converts Direct to TOKEN ID
 */

void BlockDataName(){
     match(ID);
}

void CommonBlockName(){
     match(ID);
}

void DummyArgName(){
     match(ID);
}

void EntryName(){
     match(ID);
}

void ExternalName(){
     match(ID);
}

void FunctionName(){
     match(ID);
}

void ImpliedDoVariable(){
     match(ID);
}

void IntrinsicProcedureName(){
     match(ID);
}

void ObjectName(){
     match(ID);
}

void ProgramName(){
     match(ID);
}

void SFDummyArgName(){
     match(ID);
}

void SFVarName(){
     match(ID);
}

void SubroutineName(){
     match(ID);
}

void SubroutineNameUse(){
     match(ID);
}

void VariableName(){
     match(ID);
}

void ScalarVariable(){
     VariableName();
     if (lookahead == '('){
                   match('(');
                   SectionSubscriptList();
                   match(')');
     }
}
/*
 *   Procedure Under Construction
 *
void Variable(){
     VariableName();
     match('(');
*/

void SubscriptList(){
     Subscript();
     if (lookahead == ','){
                   match(',');
                   SubscriptList();
     }
}
     

void Subscript(){
     Expr();
}
     
void LblDef(){
     if (lookahead == LABEL){
                   Label();
     }
}

/*
 *   End of Grammar and Sintatic Analyser
 */
