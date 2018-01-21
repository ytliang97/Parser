/*main*/
procedure LLPARSER(ts)
    call PUSH(S)
    accepted <- false
    while not accepted do
        if TOS() belongs sigma
        then
            call MATCH(ts, TOS())
            if TOS() = $
            then accepted <- true
            call POP()
        else
            p <- LLtable[TOS(), ts.PEEK()]
            if p = 0
            then call ERROR(syntax error)
        else call APPLY(p)
end

/*sub*/
//--STACK
procedure APPLY(p: A -> X1...Xm)
    call POP()
    for i = m downto 1 do
        call PUSH(Xi)
end

procedure MATCH(ts.token)
    if ts.PEEK() = token
    then call ts.ADVANCE()
    else call ERROR(Expected token)
end

function TOS()//...

function POP()//...

function PUSH()//...

function ADVANCE()//...Advance the input by one token

function ERROR()//...

//--Table
procedure FILLTABLE(LLtable)
    foreach A belongs N do
        foreach alpha belongs sigma do LLtable[A][alpha] <- 0
    f
     
    oreach A belongs N do
        foreach p belongs PRODUCTIONSFOR(A) do
            foreach alpha belongs Predict(p) do LLtable[A][alpha] <- p
end

function PEEK()//...examine the next input token without advancing the input
function PRODUCTIONSFOR()//...

//--Predict set
function Predict(p: A -> X1...Xm): Set
    ans <- First(X1...Xm)
    if RuleDerivesEmpty(p)
    then ans <- ans U Follow(A)
    return (ans)
end

//--First set
function FIRST(alpha) return Set
    foreach A belongs NONTERMINALS() do VisitedFirst(A) <- false
    ans <- INTERNALFIRST(alpha)
    return (ans)
end

function INTERNALFIRST(Xbeta) return Set
    if Xbeta = 
    then return (phi)
    if X belongs sigma
    then return ({X})
    /* X is a non-terminal */
    ans <- phi
    if not VisitedFirst(X)
    then
        VisitedFirst(X) <- true
        foreach rhs belongs PRODUCTIONSFOR(X) do 
            ans <- ans U INTERNALFIRST(rhs)
    if SymbolDerivesEmpty(X)
    then ans <- ans U INTERNALFIRST(beta)
    return (ans)
end

set NONTERMINALS()//...

function VisitedFirst(X)//...

function SymbolDerivesEmpty(X) return boolean//...

//--Follow set
function FOLLOW(A) return Set
    foreach A belongs NONTERMINALS() do
        VisitedFollow(A) <- false
    ans <- INTERNALFOLLOW(A)
    return (ans)
end

function INTERNALFOLLOW(A) return Set
    ans <- phi
    if not VisitedFollow(A)
    then
        VisitedFollow(A) <- true
        foreach alpha belongs OCCURRENCES(A) do 
            ans <- ans U FIRST(TAIL(alpha))
            if ALLDERIVEEMPTY(TAIL(alpha))
            then
                targ <- LHS(PRODUCTION(alpha))
                ans <- ans U INTERNALFOLLOW(targ)
    return (ans)
end

function ALLDERIVEEMPTY(gamma) return Boolean
    foreach X belongs gamma do
        if not SymbolDerivesEmpty(X) or X belongs sigma
        then return (false)
    return (true)
end

function OCCURRENCES()//...Return an iterator that visits each occurrence of X in the RHS of all rules
function PRODUCTION(y)//...reduce
function TAIL(y)//...return string behind y

//--derive lamda
procedure DERIVESEMPTYSTRING()
    foreach A belongs NONTERMINALS() do
        SymbolDerivesEmpty(A) <- false
    foreach p belongs PRODUCTIONS() do
        RuleDerivesEmpty(p) <- false
        Count(p) <- 0
        foreach X belongs RHS(p) do Count(p) <- Count(p) + 1
        call CHECKFOREMPTY(p)
    foreach X belongs WorkList do
        WorkList <- WorkList - {X}
        foreach x belongs OCCURRENCES(X) do
            p <- PRODUCTION(x)
            Count(p) <- Count(p) - 1
            call CHECKFOREMPTY(p)
end

procedure CHECKFOREMPTY(p)
    if Count(p) = 0
    then
        RuleDerivesEmpty(p) <- true
        A <- LHS(p)
        if not SymbolDerivesEmpty(A)
        then
            SymBolDerivesEmpty(A) <- true
            WorkList <- WorkList U {A}
end

set LHS()//...
set RHS()//...