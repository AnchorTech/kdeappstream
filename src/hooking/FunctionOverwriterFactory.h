#ifndef FUNCTIONOVERWRITERFACTORY_H
#define FUNCTIONOVERWRITERFACTORY_H

class AbstractFunctionOverwriter;

class FunctionOverwriterFactory
{
        static AbstractFunctionOverwriter * overwriter;

        FunctionOverwriterFactory() {}

    public:

        static AbstractFunctionOverwriter * createFunctionOverwriter();

};

#endif // FUNCTIONOVERWRITERFACTORY_H
