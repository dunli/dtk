/* dtkComposerFactory.cpp ---
 *
 * Author: Nicolas Niclausse
 * Copyright (C) 2012 - Nicolas Niclausse, Inria.
 * Created: 2012/01/30 10:37:32
 * Version: $Id$
 * Last-Updated: Thu Sep 13 16:04:16 2012 (+0200)
 *           By: tkloczko
 *     Update #: 739
 */

/* Commentary:
 *
 */

/* Change log:
 *
 */

#include <dtkConfig.h>

#include "dtkComposerFactory.h"
#include "dtkComposerNode.h"
// #include "dtkComposerNodeArrayData.h"
// #include "dtkComposerNodeArrayDataExtractor.h"
// #include "dtkComposerNodeArrayDataOperatorModifier.h"
#include "dtkComposerNodeArrayScalar.h"
#include "dtkComposerNodeArrayScalarExtractor.h"
#include "dtkComposerNodeArrayScalarOperatorModifier.h"
#include "dtkComposerNodeBoolean.h"
#include "dtkComposerNodeBooleanOperator.h"
#include "dtkComposerNodeConstants.h"
#include "dtkComposerNodeContainerData.h"
#include "dtkComposerNodeComposite.h"
#include "dtkComposerNodeControlCase.h"
#include "dtkComposerNodeControlDoWhile.h"
#include "dtkComposerNodeControlIf.h"
#include "dtkComposerNodeControlFor.h"
#include "dtkComposerNodeControlForEach.h"
#include "dtkComposerNodeControlMap.h"
#include "dtkComposerNodeControlWhile.h"
// #include "dtkComposerNodeData.h"
// #include "dtkComposerNodeFile.h"
// #include "dtkComposerNodeFileOperator.h"
#include "dtkComposerNodeLogger.h"
#include "dtkComposerNodeInteger.h"
// #include "dtkComposerNodeMatrixSquareReal.h"
// #include "dtkComposerNodeMatrixSquareRealExtractor.h"
// #include "dtkComposerNodeMatrixSquareRealOperatorUnary.h"
// #include "dtkComposerNodeMatrixSquareRealOperatorBinary.h"
#include "dtkComposerNodeNumberOperator.h"
// #include "dtkComposerNodeProcess.h"
#include "dtkComposerNodeQuaternion.h"
// #include "dtkComposerNodeQuaternionOperatorUnary.h"
// #include "dtkComposerNodeQuaternionOperatorBinary.h"
#include "dtkComposerNodeReal.h"
#include "dtkComposerNodeString.h"
#include "dtkComposerNodeStringOperator.h"
#include "dtkComposerNodeVector3D.h"
#include "dtkComposerNodeVector3DOperatorUnary.h"
#include "dtkComposerNodeVector3DOperatorBinary.h"
#include "dtkComposerNodeVector3DOperatorTernary.h"
// #include "dtkComposerNodeVectorReal.h"
// #include "dtkComposerNodeVectorRealExtractor.h"
// #include "dtkComposerNodeVectorRealOperatorModifier.h"
// #include "dtkComposerNodeVectorRealOperatorUnary.h"
// #include "dtkComposerNodeVectorRealOperatorBinary.h"
// #include "dtkComposerNodeView.h"
// #include "dtkComposerNodeRemote.h"
#include "dtkComposerSceneNodeLeaf.h"

// #if defined(DTK_HAVE_MPI)
// #include "dtkComposerNodeDistributed.h"
// #include "dtkComposerNodeWorld.h"
// #endif

// #if defined(DTK_HAVE_NITE)
// #include "dtkComposerNodeTrackerKinect.h"
// #endif

// #if defined(DTK_HAVE_VRPN)
// #include "dtkComposerNodeTrackerVrpn.h"
// #endif

// #if defined(DTK_HAVE_PLOT)
// #include "dtkComposerNodePlotCurve.h"
// #include "dtkComposerNodePlotView.h"
// #endif

// #if defined(DTK_HAVE_PLOT)
// #include <dtkPlot/dtkPlotView.h>
// #endif

#include <dtkCore/dtkAbstractView.h>
#include <dtkCore/dtkAbstractViewFactory.h>
#include <dtkCore/dtkGlobal.h>

class dtkComposerFactoryPrivate
{
public:
    QList<QString> nodes;
    QHash<QString, QString> descriptions;
    QHash<QString, QStringList> tags;
    QHash<QString, QString> types;
};

dtkComposerFactory::dtkComposerFactory(void) : d(new dtkComposerFactoryPrivate)
{
    // constant nodes

    d->nodes << "Pi";
    d->descriptions["Pi"] = dtkReadFile(":dtkComposer/dtkComposerNodePi.html");
    d->tags["Pi"] = QStringList() << "constant" << "pi";
    d->types["Pi"] = "pi";

    d->nodes << "E";
    d->descriptions["E"] = dtkReadFile(":dtkComposer/dtkComposerNodeE.html");
    d->tags["E"] = QStringList() << "constant" << "e";
    d->types["E"] = "e";

    // primitive nodes

    d->nodes << "Boolean";
    d->descriptions["Boolean"] = "<p>Description not yet filled!</p>";
    d->tags["Boolean"] = QStringList() << "primitive" << "boolean";
    d->types["Boolean"] = "boolean";

    d->nodes << "Integer";
    d->descriptions["Integer"] = "<p>Description not yet filled!</p>";
    d->tags["Integer"] = QStringList() << "primitive" << "integer" << "number";
    d->types["Integer"] = "integer";

    d->nodes << "Real";
    d->descriptions["Real"] = "<p>Description not yet filled!</p>";
    d->tags["Real"] = QStringList() << "primitive" << "real" << "number";
    d->types["Real"] = "real";

    d->nodes << "String";
    d->descriptions["String"] = "<p>Description not yet filled!</p>";
    d->tags["String"] = QStringList() << "primitive" << "string";
    d->types["String"] = "string";

    // d->nodes << "File";
    // d->descriptions["File"] = "<p>Description not yet filled!</p>";
    // d->tags["File"] = QStringList() << "primitive" << "file";
    // d->types["File"] = "file";

    // Number operators

    this->initNodeNumberOperatorUnary();
    this->initNodeNumberOperatorBinary();

    // String operators

    d->nodes << "Append";
    d->descriptions["Append"] = "<p>Description not yet filled!</p>";
    d->tags["Append"] = QStringList() << "concatenate" << "operator" << "append" << "string";
    d->types["Append"] = "append";

    // Boolean operators
    
    this->initNodeBooleanOperators();

    // Scalar Array

    this->initNodeArrayScalar();

    // Algebraic nodes

    this->initNodeVectorReal();
    this->initNodeMatrixSquareReal();

    this->initNodeVector3D();
    this->initNodeQuaternion();

    // Data Container nodes

    this->initNodeContainerData();

    // // control nodes

    this->initNodeControl();

    // log nodes

    d->nodes << "Logger";
    d->descriptions["Logger"] = "<p>Description not yet filled!</p>";
    d->tags["Logger"] = QStringList() << "logger" << "debug";
    d->types["Logger"] = "logger";

    // generic nodes

//     d->nodes << "Generic Data";
//     d->descriptions["Generic Data"] = "<p>Description not yet filled!</p>";
//     d->tags["Generic Data"] = QStringList() << "data";
//     d->types["Generic Data"] = "data";

//     d->nodes << "Generic Process";
//     d->descriptions["Generic Process"] = "<p>Description not yet filled!</p>";
//     d->tags["Generic Process"] = QStringList() << "process";
//     d->types["Generic Process"] = "process";

//     d->nodes << "Generic View";
//     d->descriptions["Generic View"] = "<p>Description not yet filled!</p>";
//     d->tags["Generic View"] = QStringList() << "view";
//     d->types["Generic View"] = "view";

//     // dtkDistributed nodes

//     d->nodes << "Remote";
//     d->tags["Remote"] = QStringList() <<  "distributed" << "tcp" << "remote" << "world";
//     d->types["Remote"] = "remote";

//     d->nodes << "Remote Submit";
//     d->tags["Remote Submit"] = QStringList() <<  "distributed" << "tcp" << "remote" << "submit" << "job";
//     d->types["Remote Submit"] = "remoteSubmit";

//     // /////////////////////////////////////////////////////////////////
//     // Plot nodes
//     // /////////////////////////////////////////////////////////////////

// #if defined(DTK_HAVE_PLOT)
//     d->nodes << "Plot Curve";
//     d->tags["Plot Curve"] = QStringList() <<  "curve" << "plot";
//     d->types["Plot Curve"] = "dtkPlotCurve";

//     dtkAbstractViewFactory::instance()->registerViewType("dtkPlotView", createPlotView);

//     d->nodes << "Plot View";
//     d->tags["Plot View"] = QStringList() <<  "view" << "plot";
//     d->types["Plot View"] = "dtkPlotView";
// #endif

//     // /////////////////////////////////////////////////////////////////
//     // NITE nodes
//     // /////////////////////////////////////////////////////////////////

// #if defined(DTK_HAVE_NITE)
//     d->nodes << "KinectTracker";
//     d->tags["KinectTracker"] = QStringList() <<  "kinect" << "vr" << "ar" << "tracker";
//     d->types["KinectTracker"] = "kinectTracker";
// #endif

//     // /////////////////////////////////////////////////////////////////
//     // VRPN nodes
//     // /////////////////////////////////////////////////////////////////

// #if defined(DTK_HAVE_VRPN)
//     d->nodes << "VrpnTracker";
//     d->tags["VrpnTracker"] = QStringList() <<  "vrpn" << "vr" << "ar" << "tracker";
//     d->types["VrpnTracker"] = "vrpnTracker";
// #endif

//     // /////////////////////////////////////////////////////////////////
//     // MPI nodes
//     // /////////////////////////////////////////////////////////////////

// #if defined(DTK_HAVE_MPI)
//     d->nodes << "World";
//     d->tags["World"] = QStringList() <<  "distributed" << "mpi" << "tcp" << "world";
//     d->types["World"] = "world";

//     d->nodes << "CommunicatorRank";
//     d->tags["CommunicatorRank"] = QStringList() <<  "rank" << "distributed" << "mpi" << "communicator";
//     d->types["CommunicatorRank"] = "communicatorRank";

//     d->nodes << "CommunicatorSize";
//     d->tags["CommunicatorSize"] = QStringList() <<  "size" << "distributed" << "mpi" << "communicator";
//     d->types["CommunicatorSize"] = "communicatorSize";

//     d->nodes << "CommunicatorInit";
//     d->tags["CommunicatorInit"] = QStringList() <<  "initialization" << "distributed" << "mpi" << "communicator";
//     d->types["CommunicatorInit"] = "communicatorInit";

//     d->nodes << "CommunicatorUninitialize";
//     d->tags["CommunicatorUninitialize"] = QStringList() <<  "finalize" << "distributed" << "mpi" << "communicator";
//     d->types["CommunicatorUninitialize"] = "communicatorUninitialize";

//     d->nodes << "CommunicatorSendInteger";
//     d->tags["CommunicatorSendInteger"] = QStringList() <<  "send" << "distributed" << "mpi" << "communicator" << "integer";
//     d->types["CommunicatorSendInteger"] = "communicatorSendInteger";

//     d->nodes << "CommunicatorReceiveInteger";
//     d->tags["CommunicatorReceiveInteger"] = QStringList() <<  "receive" << "distributed" << "mpi" << "communicator" << "integer";;
//     d->types["CommunicatorReceiveInteger"] = "communicatorReceiveInteger";

//     d->nodes << "CommunicatorSendReal";
//     d->tags["CommunicatorSendReal"] = QStringList() <<  "send" << "distributed" << "mpi" << "communicator" << "real";
//     d->types["CommunicatorSendReal"] = "communicatorSendReal";

//     d->nodes << "CommunicatorReceiveReal";
//     d->tags["CommunicatorReceiveReal"] = QStringList() <<  "receive" << "distributed" << "mpi" << "communicator" << "real";;
//     d->types["CommunicatorReceiveReal"] = "communicatorReceiveReal";

//     d->nodes << "CommunicatorReceive";
//     d->tags["CommunicatorReceive"] = QStringList() <<  "receive" << "distributed" << "mpi" << "communicator";;
//     d->types["CommunicatorReceive"] = "communicatorReceive";

//     d->nodes << "CommunicatorSend";
//     d->tags["CommunicatorSend"] = QStringList() <<  "send" << "distributed" << "mpi" << "communicator";;
//     d->types["CommunicatorSend"] = "communicatorSend";
// #endif
}

void dtkComposerFactory::initNodeNumberOperatorUnary()
{
    d->nodes << "Abs";
    d->descriptions["Abs"] = "<p>Returns absolute value of the input number.</p>";
    d->tags["Abs"] = QStringList() << "number" << "operator" << "unary" << "abs";
    d->types["Abs"] = "abs";

    d->nodes << "Incr";
    d->descriptions["Incr"] = "<p>Increments the input number.</p>";
    d->tags["Incr"] = QStringList() << "number" << "operator" << "unary" << "incr";
    d->types["Incr"] = "incr";

    d->nodes << "Decr";
    d->descriptions["Decr"] = "<p>Decrements the input number.</p>";
    d->tags["Decr"] = QStringList() << "number" << "operator" << "unary" << "decr";
    d->types["Decr"] = "decr";

    d->nodes << "Opp";
    d->descriptions["Opp"] = "<p>Returns opposite of the input number.</p>";
    d->tags["Opp"] = QStringList() << "number" << "operator" << "unary" << "opp";
    d->types["Opp"] = "opp";

    d->nodes << "Inv";
    d->descriptions["Inv"] = "<p>Returns inverse of the input number.</p>";
    d->tags["Inv"] = QStringList() << "number" << "operator" << "unary" << "inv";
    d->types["Inv"] = "inv";

    d->nodes << "Square";
    d->descriptions["Square"] = "<p>Returns square of the input number.</p>";
    d->tags["Square"] = QStringList() << "number" << "operator" << "unary" << "square";
    d->types["Square"] = "square";

    d->nodes << "Sqrt";
    d->descriptions["Sqrt"] = "<p>Returns square root of the input number.</p>";
    d->tags["Sqrt"] = QStringList() << "number" << "operator" << "unary" << "sqrt";
    d->types["Sqrt"] = "sqrt";

    d->nodes << "Ln";
    d->descriptions["Ln"] = "<p>Returns the natural logarithm of the input number.</p>";
    d->tags["Ln"] = QStringList() << "number" << "operator" << "unary" << "ln";
    d->types["Ln"] = "ln";

    d->nodes << "Log10";
    d->descriptions["Log10"] = "<p>Returns the binary logarithm in base 10 of the input number.</p>";
    d->tags["Log10"] = QStringList() << "number" << "operator" << "unary" << "log10";
    d->types["Log10"] = "log10";

    d->nodes << "Exp";
    d->descriptions["Exp"] = "<p>Returns the exponential value of the input number.</p>";
    d->tags["Exp"] = QStringList() << "number" << "operator" << "unary" << "exp";
    d->types["Exp"] = "exp";

    d->nodes << "Cos";
    d->descriptions["Cos"] = "<p>Returns the cosine of the input number.</p>";
    d->tags["Cos"] = QStringList() << "number" << "operator" << "unary" << "cos";
    d->types["Cos"] = "cos";

    d->nodes << "Sin";
    d->descriptions["Sin"] = "<p>Returns the sine of the input number.</p>";
    d->tags["Sin"] = QStringList() << "number" << "operator" << "unary" << "sin";
    d->types["Sin"] = "sin";

    d->nodes << "Tan";
    d->descriptions["Tan"] = "<p>Returns the tangent of the input number.</p>";
    d->tags["Tan"] = QStringList() << "number" << "operator" << "unary" << "tan";
    d->types["Tan"] = "tan";

    d->nodes << "Acos";
    d->descriptions["Acos"] = "<p>Description not yet filled!</p>";
    d->tags["Acos"] = QStringList() << "number" << "operator" << "unary" << "acos";
    d->types["Acos"] = "acos";

    d->nodes << "Asin";
    d->descriptions["Asin"] = "<p>Description not yet filled!</p>";
    d->tags["Asin"] = QStringList() << "number" << "operator" << "unary" << "asin";
    d->types["Asin"] = "asin";

    d->nodes << "Atan";
    d->descriptions["Atan"] = "<p>Description not yet filled!</p>";
    d->tags["Atan"] = QStringList() << "number" << "operator" << "unary" << "atan";
    d->types["Atan"] = "atan";

    d->nodes << "Deg2rad";
    d->descriptions["Deg2rad"] = "<p>Description not yet filled!</p>";
    d->tags["Deg2rad"] = QStringList() << "number" << "operator" << "unary" << "deg2rad";
    d->types["Deg2rad"] = "deg2rad";

    d->nodes << "Rad2deg";
    d->descriptions["Rad2deg"] = "<p>Description not yet filled!</p>";
    d->tags["Rad2deg"] = QStringList() << "number" << "operator" << "unary" << "rad2deg";
    d->types["Rad2deg"] = "rad2deg";

    d->nodes << "Ceil";
    d->descriptions["Ceil"] = "<p>Description not yet filled!</p>";
    d->tags["Ceil"] = QStringList() << "number" << "operator" << "unary" << "ceil";
    d->types["Ceil"] = "ceil";

    d->nodes << "Floor";
    d->descriptions["Floor"] = "<p>Description not yet filled!</p>";
    d->tags["Floor"] = QStringList() << "number" << "operator" << "unary" << "floor";
    d->types["Floor"] = "floor";

    d->nodes << "Round";
    d->descriptions["Round"] = "<p>Description not yet filled!</p>";
    d->tags["Round"] = QStringList() << "number" << "operator" << "unary" << "round";
    d->types["Round"] = "round";
}

void dtkComposerFactory::initNodeNumberOperatorBinary(void)
{
    d->nodes << "String Equality";
    d->descriptions["String Equality"] = "<p>Description not yet filled!</p>";
    d->tags["String Equality"] = QStringList() << "string" << "comparison" << "equality";
    d->types["String Equality"] = "string_equality";

    d->nodes << "Eucldiv";
    d->descriptions["Eucldiv"] = "<p>Description not yet filled!</p>";
    d->tags["Eucldiv"] = QStringList() << "number" << "operator" << "binary" << "eucldiv";
    d->types["Eucldiv"] = "eucldiv";

    d->nodes << "Expn";
    d->descriptions["Expn"] = "<p>Description not yet filled!</p>";
    d->tags["Expn"] = QStringList() << "number" << "operator" << "binary" << "expn";
    d->types["Expn"] = "expn";

    d->nodes << "Logn";
    d->descriptions["Logn"] = "<p>Description not yet filled!</p>";
    d->tags["Logn"] = QStringList() << "number" << "operator" << "binary" << "logn";
    d->types["Logn"] = "logn";

    d->nodes << "Max";
    d->descriptions["Max"] = "<p>Description not yet filled!</p>";
    d->tags["Max"] = QStringList() << "number" << "operator" << "binary" << "max";
    d->types["Max"] = "max";

    d->nodes << "Minus";
    d->descriptions["Minus"] = "<p>Description not yet filled!</p>";
    d->tags["Minus"] = QStringList() << "number" << "operator" << "binary" << "minus";
    d->types["Minus"] = "minus";

    d->nodes << "Min";
    d->descriptions["Min"] = "<p>Description not yet filled!</p>";
    d->tags["Min"] = QStringList() << "number" << "operator" << "binary" << "min";
    d->types["Min"] = "min";

    d->nodes << "Modulo";
    d->descriptions["Modulo"] = "<p>Description not yet filled!</p>";
    d->tags["Modulo"] = QStringList() << "number" << "operator" << "binary" << "modulo";
    d->types["Modulo"] = "modulo";

    d->nodes << "Mult";
    d->descriptions["Mult"] = "<p>Description not yet filled!</p>";
    d->tags["Mult"] = QStringList() << "number" << "operator" << "binary" << "mult";
    d->types["Mult"] = "mult";

    d->nodes << "Plus";
    d->descriptions["Plus"] = "<p>Description not yet filled!</p>";
    d->tags["Plus"] = QStringList() << "number" << "operator" << "binary" << "plus";
    d->types["Plus"] = "plus";

    d->nodes << "Posnthroot";
    d->descriptions["Posnthroot"] = "<p>Description not yet filled!</p>";
    d->tags["Posnthroot"] = QStringList() << "number" << "operator" << "binary" << "posnthroot";
    d->types["Posnthroot"] = "posnthroot";

    d->nodes << "Power";
    d->descriptions["Power"] = "<p>Description not yet filled!</p>";
    d->tags["Power"] = QStringList() << "number" << "operator" << "binary" << "power";
    d->types["Power"] = "power";

    d->nodes << "Ratio";
    d->descriptions["Ratio"] = "<p>Description not yet filled!</p>";
    d->tags["Ratio"] = QStringList() << "number" << "operator" << "binary" << "ratio";
    d->types["Ratio"] = "ratio";

    d->nodes << "Equal";
    d->descriptions["Equal"] = "<p>Description not yet filled!</p>";
    d->tags["Equal"] = QStringList() << "number" << "operator" << "binary" << "equal";
    d->types["Equal"] = "equal";

    d->nodes << "Notequal";
    d->descriptions["Notequal"] = "<p>Description not yet filled!</p>";
    d->tags["Notequal"] = QStringList() << "number" << "operator" << "binary" << "notequal";
    d->types["Notequal"] = "notequal";

    d->nodes << "Gt";
    d->descriptions["Gt"] = "<p>Description not yet filled!</p>";
    d->tags["Gt"] = QStringList() << "number" << "operator" << "binary" << "gt";
    d->types["Gt"] = "gt";

    d->nodes << "Lt";
    d->descriptions["Lt"] = "<p>Description not yet filled!</p>";
    d->tags["Lt"] = QStringList() << "number" << "operator" << "binary" << "lt";
    d->types["Lt"] = "lt";

    d->nodes << "Gte";
    d->descriptions["Gte"] = "<p>Description not yet filled!</p>";
    d->tags["Gte"] = QStringList() << "number" << "operator" << "binary" << "gte";
    d->types["Gte"] = "gte";

    d->nodes << "Lte";
    d->descriptions["Lte"] = "<p>Description not yet filled!</p>";
    d->tags["Lte"] = QStringList() << "number" << "operator" << "binary" << "lte";
    d->types["Lte"] = "lte";

    d->nodes << "Almosteq";
    d->descriptions["Almosteq"] = "<p>Description not yet filled!</p>";
    d->tags["Almosteq"] = QStringList() << "number" << "operator" << "binary" << "almosteq";
    d->types["Almosteq"] = "almosteq";

    d->nodes << "Notalmosteq";
    d->descriptions["Notalmosteq"] = "<p>Description not yet filled!</p>";
    d->tags["Notalmosteq"] = QStringList() << "number" << "operator" << "binary" << "notalmosteq";
    d->types["Notalmosteq"] = "notalmosteq";

}

void dtkComposerFactory::initNodeBooleanOperators(void)
{
    d->nodes << "Not";
    d->descriptions["Not"] = "<p>Description not yet filled!</p>";
    d->tags["Not"] = QStringList() << "boolean" << "operator" << "unary" << "not";
    d->types["Not"] = "not";

    d->nodes << "And";
    d->descriptions["And"] = "<p>Description not yet filled!</p>";
    d->tags["And"] = QStringList() << "boolean" << "operator" << "binary" << "and";
    d->types["And"] = "and";

    d->nodes << "Or";
    d->descriptions["Or"] = "<p>Description not yet filled!</p>";
    d->tags["Or"] = QStringList() << "boolean" << "operator" << "binary" << "or";
    d->types["Or"] = "or";

    d->nodes << "Xor";
    d->descriptions["Xor"] = "<p>Description not yet filled!</p>";
    d->tags["Xor"] = QStringList() << "boolean" << "operator" << "binary" << "xor";
    d->types["Xor"] = "xor";

    d->nodes << "Nand";
    d->descriptions["Nand"] = "<p>Description not yet filled!</p>";
    d->tags["Nand"] = QStringList() << "boolean" << "operator" << "binary" << "nand";
    d->types["Nand"] = "nand";

    d->nodes << "Nor";
    d->descriptions["Nor"] = "<p>Description not yet filled!</p>";
    d->tags["Nor"] = QStringList() << "boolean" << "operator" << "binary" << "nor";
    d->types["Nor"] = "nor";

    d->nodes << "Xnor";
    d->descriptions["Xnor"] = "<p>Description not yet filled!</p>";
    d->tags["Xnor"] = QStringList() << "boolean" << "operator" << "binary" << "xnor";
    d->types["Xnor"] = "xnor";

    d->nodes << "Imp";
    d->descriptions["Imp"] = "<p>Description not yet filled!</p>";
    d->tags["Imp"] = QStringList() << "boolean" << "operator" << "binary" << "imp";
    d->types["Imp"] = "imp";

    d->nodes << "Nimp";
    d->descriptions["Nimp"] = "<p>Description not yet filled!</p>";
    d->tags["Nimp"] = QStringList() << "boolean" << "operator" << "binary" << "nimp";
    d->types["Nimp"] = "nimp";
}

void dtkComposerFactory::initNodeVectorReal(void)
{
    // d->nodes << "Vector Real";
    // d->descriptions["Vector Real"] = "<p>Description not yet filled!</p>";
    // d->tags["Vector Real"] = QStringList() << "vector" << "real" << "algebraic";
    // d->types["Vector Real"] = "vector_real";

    // d->nodes << "Vector Real Extractor";
    // d->descriptions["Vector Real Extractor"] = "<p>Description not yet filled!</p>";
    // d->tags["Vector Real Extractor"] = QStringList() << "vector" << "real" << "extractor";
    // d->types["Vector Real Extractor"] = "vector_real_extractor";

    // d->nodes << "Vector Real Set";
    // d->descriptions["Vector Real Set"] = "<p>Description not yet filled!</p>";
    // d->tags["Vector Real Set"] = QStringList() << "vector" << "real" << "set";
    // d->types["Vector Real Set"] = "vector_real_set";

    // d->nodes << "Vector Real Sum";
    // d->descriptions["Vector Real Sum"] = "<p>Description not yet filled!</p>";
    // d->tags["Vector Real Sum"] = QStringList() << "vector" << "real" << "sum";
    // d->types["Vector Real Sum"] = "vector_real_sum";

    // d->nodes << "Vector Real Substract";
    // d->descriptions["Vector Real Substract"] = "<p>Description not yet filled!</p>";
    // d->tags["Vector Real Substract"] = QStringList() << "vector" << "real" << "substract";
    // d->types["Vector Real Substract"] = "vector_real_substract";

    // d->nodes << "Vector Real Mult";
    // d->descriptions["Vector Real Mult"] = "<p>Description not yet filled!</p>";
    // d->tags["Vector Real Mult"] = QStringList() << "vector" << "real" << "mult";
    // d->types["Vector Real Mult"] = "vector_real_mult";

    // d->nodes << "Vector Real Divide";
    // d->descriptions["Vector Real Divide"] = "<p>Description not yet filled!</p>";
    // d->tags["Vector Real Divide"] = QStringList() << "vector" << "real" << "divide";
    // d->types["Vector Real Divide"] = "vector_real_divide";

    // d->nodes << "Vector Real Add All";
    // d->descriptions["Vector Real Add All"] = "<p>Description not yet filled!</p>";
    // d->tags["Vector Real Add All"] = QStringList() << "vector" << "real" << "add"<< "all";
    // d->types["Vector Real Add All"] = "vector_real_add_all";

    // d->nodes << "Vector Real Substract All";
    // d->descriptions["Vector Real Substract All"] = "<p>Description not yet filled!</p>";
    // d->tags["Vector Real Substract All"] = QStringList() << "vector" << "real" << "substract"<< "all";
    // d->types["Vector Real Substract All"] = "vector_real_substract_all";

    // d->nodes << "Vector Real Mult All";
    // d->descriptions["Vector Real Mult All"] = "<p>Description not yet filled!</p>";
    // d->tags["Vector Real Mult All"] = QStringList() << "vector" << "real" << "mult"<< "all";
    // d->types["Vector Real Mult All"] = "vector_real_mult_all";

    // d->nodes << "Vector Real Divide All";
    // d->descriptions["Vector Real Divide All"] = "<p>Description not yet filled!</p>";
    // d->tags["Vector Real Divide All"] = QStringList() << "vector" << "real" << "divide"<< "all";
    // d->types["Vector Real Divide All"] = "vector_real_divide_all";

    // d->nodes << "Vector Real Unit";
    // d->descriptions["Vector Real Unit"] = "<p>Description not yet filled!</p>";
    // d->tags["Vector Real Unit"] = QStringList() << "vector"<<"real" << "algebraic" << "unit";
    // d->types["Vector Real Unit"] = "vectorReal_unit";

    // d->nodes << "Vector Real Norm";
    // d->descriptions["Vector Real Norm"] = "<p>Description not yet filled!</p>";
    // d->tags["Vector Real Norm"] = QStringList() << "vector"<<"real" << "algebraic" << "norm";
    // d->types["Vector Real Norm"] = "vectorReal_norm";

    // d->nodes << "Vector Real Sum";
    // d->descriptions["Vector Real Sum"] = "<p>Description not yet filled!</p>";
    // d->tags["Vector Real Sum"] = QStringList() << "vector"<< "real" << "algebraic" << "sum";
    // d->types["Vector Real Sum"] = "vectorReal_sum";

    // d->nodes << "Vector Real Substract";
    // d->descriptions["Vector Real Substract"] = "<p>Description not yet filled!</p>";
    // d->tags["Vector Real Substract"] = QStringList() << "vector"<<"real" << "algebraic" << "substraction";
    // d->types["Vector Real Substract"] = "vectorReal_substract";

    // d->nodes << "Vector Real Dot Prod";
    // d->descriptions["Vector Real Dot Prod"] = "<p>Description not yet filled!</p>";
    // d->tags["Vector Real Dot Prod"] = QStringList() << "vector"<<"real" << "algebraic" << "dot product";
    // d->types["Vector Real Dot Prod"] = "vectorReal_dot_prod";

    // d->nodes << "Vector Real Scal Mult";
    // d->descriptions["Vector Real Scal Mult"] = "<p>Description not yet filled!</p>";
    // d->tags["Vector Real Scal Mult"] = QStringList() << "vector"<<"real" << "algebraic" << "scalar multiplication";
    // d->types["Vector Real Scal Mult"] = "vectorReal_scal_mult";

    // d->nodes << "Vector Real Scal Division";
    // d->descriptions["Vector Real Scal Division"] = "<p>Description not yet filled!</p>";
    // d->tags["Vector Real Scal Division"] = QStringList() << "vector"<<"real" << "algebraic" << "scalar division";
    // d->types["Vector Real Scal Division"] = "vectorReal_scal_divide";
}

void dtkComposerFactory::initNodeMatrixSquareReal(void)
{
    // d->nodes << "Matrix Square Real";
    // d->descriptions["Matrix Square Real "] = "<p>Description not yet filled!</p>";
    // d->tags["Matrix Square Real"] = QStringList() << "matrix" << "square" << "real"<< "algebraic";
    // d->types["Matrix Square Real"] = "matrix_square_real";

    // d->nodes << "MatrixSquare Real Extractor";
    // d->descriptions["MatrixSquare Real Extractor "] = "<p>Description not yet filled!</p>";
    // d->tags["MatrixSquare Real Extractor"] = QStringList() << "matrix" << "square" << "real"<< "extractor";
    // d->types["MatrixSquare Real Extractor"] = "matrixSquare_real_extractor";

    // d->nodes << "MatrixSquare Real Transpose";
    // d->descriptions["MatrixSquare Real Transpose "] = "<p>Description not yet filled!</p>";
    // d->tags["MatrixSquare Real Transpose"] = QStringList() << "matrix" << "square" << "real"<< "transpose";
    // d->types["MatrixSquare Real Transpose"] = "matrixSquare_real_transpose";

    // d->nodes << "MatrixSquare Real Inverse";
    // d->descriptions["MatrixSquare Real Inverse "] = "<p>Description not yet filled!</p>";
    // d->tags["MatrixSquare Real Inverse"] = QStringList() << "matrix" << "square" << "real"<< "inverse";
    // d->types["MatrixSquare Real Inverse"] = "matrixSquare_real_inverse";

    // d->nodes << "MatrixSquare Real Determinant";
    // d->descriptions["MatrixSquare Real Determinant "] = "<p>Description not yet filled!</p>";
    // d->tags["MatrixSquare Real Determinant"] = QStringList() << "matrix" << "square" << "real"<< "determinant";
    // d->types["MatrixSquare Real Determinant"] = "matrixSquare_real_determinant";

    // d->nodes << "MatrixSquare Real Trace";
    // d->descriptions["MatrixSquare Real Trace "] = "<p>Description not yet filled!</p>";
    // d->tags["MatrixSquare Real Trace"] = QStringList() << "matrix" << "square" << "real"<< "trace";
    // d->types["MatrixSquare Real Trace"] = "matrixSquare_real_trace";

    // d->nodes << "MatrixSquare Real Sum";
    // d->descriptions["MatrixSquare Real Sum "] = "<p>Description not yet filled!</p>";
    // d->tags["MatrixSquare Real Sum"] = QStringList() << "matrix" << "square" << "real"<< "sum";
    // d->types["MatrixSquare Real Sum"] = "matrixSquare_real_sum";

    // d->nodes << "MatrixSquare Real Substract";
    // d->descriptions["MatrixSquare Real Substract "] = "<p>Description not yet filled!</p>";
    // d->tags["MatrixSquare Real Substract"] = QStringList() << "matrix" << "square" << "real"<< "substract";
    // d->types["MatrixSquare Real Substract"] = "matrixSquare_real_substract";

    // d->nodes << "MatrixSquare Real Mult";
    // d->descriptions["MatrixSquare Real Mult "] = "<p>Description not yet filled!</p>";
    // d->tags["MatrixSquare Real Mult"] = QStringList() << "matrix" << "square" << "real"<< "mult";
    // d->types["MatrixSquare Real Mult"] = "matrixSquare_real_mult";

    // d->nodes << "MatrixSquare Real ProductMatrixVector";
    // d->descriptions["MatrixSquare Real ProductMatrixVector "] = "<p>Description not yet filled!</p>";
    // d->tags["MatrixSquare Real ProductMatrixVector"] = QStringList() << "matrix" << "square" << "real"<< "Product"<< "Matrix" << "Vector";
    // d->types["MatrixSquare Real ProductMatrixVector"] = "matrixSquare_real_ProductMatrixVector";

    // d->nodes << "MatrixSquare Real ProductVectorMatrix";
    // d->descriptions["MatrixSquare Real ProductVectorMatrix "] = "<p>Description not yet filled!</p>";
    // d->tags["MatrixSquare Real ProductVectorMatrix"] = QStringList() << "matrix" << "square" << "real"<< "Product"<< "Matrix" << "Vector";
    // d->types["MatrixSquare Real ProductVectorMatrix"] = "matrixSquare_real_ProductVectorMatrix";

    // d->nodes << "MatrixSquare Real ReplaceRowMatrixByVector";
    // d->descriptions["MatrixSquare Real ReplaceRowMatrixByVector "] = "<p>Description not yet filled!</p>";
    // d->tags["MatrixSquare Real ReplaceRowMatrixByVector"] = QStringList() << "matrix" << "square" << "real"<< "Replace"<< "Row"<< "Matrix" << "Vector";
    // d->types["MatrixSquare Real ReplaceRowMatrixByVector"] = "matrixSquare_real_ReplaceRowMatrixByVector";

    // d->nodes << "MatrixSquare Real ReplaceColMatrixByVector";
    // d->descriptions["MatrixSquare Real ReplaceColMatrixByVector "] = "<p>Description not yet filled!</p>";
    // d->tags["MatrixSquare Real ReplaceColMatrixByVector"] = QStringList() << "matrix" << "square" << "real"<< "Replace"<< "Col"<< "Matrix" << "Vector";
    // d->types["MatrixSquare Real ReplaceColMatrixByVector"] = "matrixSquare_real_ReplaceColMatrixByVector";
}

void dtkComposerFactory::initNodeVector3D(void)
{
    d->nodes << "Vector3D";
    d->descriptions["Vector3D"] = "<p>Description not yet filled!</p>";
    d->tags["Vector3D"] = QStringList() << "vector3D" << "algebraic";
    d->types["Vector3D"] = "vector3D";

    d->nodes << "Vector3D Unit";
    d->descriptions["Vector3D Unit"] = "<p>Description not yet filled!</p>";
    d->tags["Vector3D Unit"] = QStringList() << "vector3D" << "algebraic" << "unit";
    d->types["Vector3D Unit"] = "vector3D_unit";

    d->nodes << "Vector3D Norm";
    d->descriptions["Vector3D Norm"] = "<p>Description not yet filled!</p>";
    d->tags["Vector3D Norm"] = QStringList() << "vector3D" << "algebraic" << "norm";
    d->types["Vector3D Norm"] = "vector3D_norm";

    d->nodes << "Vector3D Sum";
    d->descriptions["Vector3D Sum"] = "<p>Description not yet filled!</p>";
    d->tags["Vector3D Sum"] = QStringList() << "vector3D" << "algebraic" << "sum";
    d->types["Vector3D Sum"] = "vector3D_sum";

    d->nodes << "Vector3D Substract";
    d->descriptions["Vector3D Substract"] = "<p>Description not yet filled!</p>";
    d->tags["Vector3D Substract"] = QStringList() << "vector3D" << "algebraic" << "substraction";
    d->types["Vector3D Substract"] = "vector3D_substract";

    d->nodes << "Vector3D Cross Prod";
    d->descriptions["Vector3D Cross Prod"] = "<p>Description not yet filled!</p>";
    d->tags["Vector3D Cross Prod"] = QStringList() << "vector3D" << "algebraic" << "cross product";
    d->types["Vector3D Cross Prod"] = "vector3D_cross_prod";

    d->nodes << "Vector3D Dot Prod";
    d->descriptions["Vector3D Dot Prod"] = "<p>Description not yet filled!</p>";
    d->tags["Vector3D Dot Prod"] = QStringList() << "vector3D" << "algebraic" << "dot product";
    d->types["Vector3D Dot Prod"] = "vector3D_dot_prod";

    d->nodes << "Vector3D Scal Mult";
    d->descriptions["Vector3D Scal Mult"] = "<p>Description not yet filled!</p>";
    d->tags["Vector3D Scal Mult"] = QStringList() << "vector3D" << "algebraic" << "scalar multiplication";
    d->types["Vector3D Scal Mult"] = "vector3D_scal_mult";

    d->nodes << "Vector3D Scal Division";
    d->descriptions["Vector3D Scal Division"] = "<p>Description not yet filled!</p>";
    d->tags["Vector3D Scal Division"] = QStringList() << "vector3D" << "algebraic" << "scalar division";
    d->types["Vector3D Scal Division"] = "vector3D_scal_divide";

    d->nodes << "Vector3D Triple Prod";
    d->descriptions["Vector3D Triple Prod"] = "<p>Description not yet filled!</p>";
    d->tags["Vector3D Triple Prod"] = QStringList() << "vector3D" << "algebraic" << "triple product";
    d->types["Vector3D Triple Prod"] = "vector3D_triple_prod";

    d->nodes << "Vector3D Mixed Prod";
    d->descriptions["Vector3D Mixed Prod"] = "<p>Description not yet filled!</p>";
    d->tags["Vector3D Mixed Prod"] = QStringList() << "vector3D" << "algebraic" << "mixed product";
    d->types["Vector3D Mixed Prod"] = "vector3D_mixed_prod";
}

void dtkComposerFactory::initNodeQuaternion(void)
{
    d->nodes << "Quaternion";
    d->descriptions["Quaternion"] = "<p>Description not yet filled!</p>";
    d->tags["Quaternion"] = QStringList() << "quaternion" << "algebraic";
    d->types["Quaternion"] = "quaternion";

    // d->nodes << "Quaternion Unit";
    // d->descriptions["Quaternion Unit"] = "<p>Description not yet filled!</p>";
    // d->tags["Quaternion Unit"] = QStringList() << "quaternion" << "algebraic" << "unit";
    // d->types["Quaternion Unit"] = "quat_unit";

    // d->nodes << "Quaternion Norm";
    // d->descriptions["Quaternion Norm"] = "<p>Description not yet filled!</p>";
    // d->tags["Quaternion Norm"] = QStringList() << "quaternion" << "algebraic" << "norm";
    // d->types["Quaternion Norm"] = "quat_norm";

    // d->nodes << "Quaternion Sum";
    // d->descriptions["Quaternion Sum"] = "<p>Description not yet filled!</p>";
    // d->tags["Quaternion Sum"] = QStringList() << "quaternion" << "algebraic" << "sum";
    // d->types["Quaternion Sum"] = "quat_sum";

    // d->nodes << "Quaternion Substract";
    // d->descriptions["Quaternion Substract"] = "<p>Description not yet filled!</p>";
    // d->tags["Quaternion Substract"] = QStringList() << "quaternion" << "algebraic" << "substract";
    // d->types["Quaternion Substract"] = "quat_substract";

    // d->nodes << "Quaternion Mult";
    // d->descriptions["Quaternion Mult"] = "<p>Description not yet filled!</p>";
    // d->tags["Quaternion Mult"] = QStringList() << "quaternion" << "algebraic" << "multiplication";
    // d->types["Quaternion Mult"] = "quat_mult";

    // d->nodes << "Quaternion Division";
    // d->descriptions["Quaternion Division"] = "<p>Description not yet filled!</p>";
    // d->tags["Quaternion Division"] = QStringList() << "quaternion" << "algebraic" << "division";
    // d->types["Quaternion Division"] = "quat_divide";

    // d->nodes << "Quaternion Scal Mult";
    // d->descriptions["Quaternion Scal Mult"] = "<p>Description not yet filled!</p>";
    // d->tags["Quaternion Scal Mult"] = QStringList() << "quaternion" << "algebraic" << "scalar multiplication";
    // d->types["Quaternion Scal Mult"] = "quat_scal_mult";

    // d->nodes << "Quaternion Scal Division";
    // d->descriptions["Quaternion Scal Division"] = "<p>Description not yet filled!</p>";
    // d->tags["Quaternion Scal Division"] = QStringList() << "quaternion" << "algebraic" << "scalar division";
    // d->types["Quaternion Scal Division"] = "quat_scal_divide";
}

void dtkComposerFactory::initNodeArrayScalar(void)
{
    d->nodes << "Scalar Array";
    d->descriptions["Scalar Array"] = "<p>Description not yet filled!</p>";
    d->tags["Scalar Array"] = QStringList() << "container" << "array" << "scalar" ;
    d->types["Scalar Array"] = "array_scalar";

    d->nodes << "Scalar Array Extractor";
    d->descriptions["Scalar Array Extractor"] = "<p>Description not yet filled!</p>";
    d->tags["Scalar Array Extractor"] = QStringList() << "container" << "array" << "scalar" << "extractor" ;
    d->types["Scalar Array Extractor"] = "array_scalar_extractor";

    d->nodes << "Scalar Array SubArray";
    d->descriptions["Scalar Array SubArray"] = "<p>Description not yet filled!</p>";
    d->tags["Scalar Array SubArray"] = QStringList() << "container" << "scalar" << "array" << "subarray"<< "extractor" ;
    d->types["Scalar Array SubArray"] = "array_scalar_extractor_subarray";

    d->nodes << "Scalar Array Part";
    d->descriptions["Scalar Array Part"] = "<p>Description not yet filled!</p>";
    d->tags["Scalar Array Part"] = QStringList() << "container" << "scalar" << "array" << "part" << "subarray" << "extractor" ;
    d->types["Scalar Array Part"] = "array_scalar_extractor_array_part";

    d->nodes << "Scalar Array Insert";
    d->descriptions["Scalar Array Insert"] = "<p>Description not yet filled!</p>";
    d->tags["Scalar Array Insert"] = QStringList() << "container" << "array" << "scalar"  << "insert" ;
    d->types["Scalar Array Insert"] = "array_scalar_insert";

    d->nodes << "Scalar Array Set";
    d->descriptions["Scalar Array Set"] = "<p>Description not yet filled!</p>";
    d->tags["Scalar Array Set"] = QStringList() << "container" << "array" << "scalar"  << "set" ;
    d->types["Scalar Array Set"] = "array_scalar_set";

    d->nodes << "Scalar Array Append";
    d->descriptions["Scalar Array Append"] = "<p>Description not yet filled!</p>";
    d->tags["Scalar Array Append"] = QStringList() << "container" << "array" << "scalar"  << "append" ;
    d->types["Scalar Array Append"] = "array_scalar_append";

    d->nodes << "Scalar Array Prepend";
    d->descriptions["Scalar Array Prepend"] = "<p>Description not yet filled!</p>";
    d->tags["Scalar Array Prepend"] = QStringList() << "container" << "array" << "scalar"  << "prepend" ;
    d->types["Scalar Array Prepend"] = "array_scalar_prepend";

    d->nodes << "Scalar Array All Add";
    d->descriptions["Scalar Array All Add"] = "<p>Description not yet filled!</p>";
    d->tags["Scalar Array All Add"] = QStringList() << "container" << "array" << "scalar"  << "all"<< "add" ;
    d->types["Scalar Array All Add"] = "array_scalar_all_add";

    d->nodes << "Scalar Array All Substract";
    d->descriptions["Scalar Array All Substract"] = "<p>Description not yet filled!</p>";
    d->tags["Scalar Array All Substract"] = QStringList() << "container" << "array" << "scalar"  << "all"<< "substract" ;
    d->types["Scalar Array All Substract"] = "array_scalar_all_substract";

    d->nodes << "Scalar Array All Mult";
    d->descriptions["Scalar Array All Mult"] = "<p>Description not yet filled!</p>";
    d->tags["Scalar Array All Mult"] = QStringList() << "container" << "array" << "scalar"  << "all"<< "mult" ;
    d->types["Scalar Array All Mult"] = "array_scalar_all_mult";

    d->nodes << "Scalar Array All Divide";
    d->descriptions["Scalar Array All Divide"] = "<p>Description not yet filled!</p>";
    d->tags["Scalar Array All Divide"] = QStringList() << "container" << "array" << "scalar"  << "all"<< "divide" ;
    d->types["Scalar Array All Divide"] = "array_scalar_all_divide";

    d->nodes << "Scalar Array Sum";
    d->descriptions["Scalar Array Sum"] = "<p>Description not yet filled!</p>";
    d->tags["Scalar Array Sum"] = QStringList() << "container" << "array" << "scalar"  << "sum" ;
    d->types["Scalar Array Sum"] = "array_scalar_sum";

    d->nodes << "Scalar Array Substract";
    d->descriptions["Scalar Array Substract"] = "<p>Description not yet filled!</p>";
    d->tags["Scalar Array Substract"] = QStringList() << "container" << "array" << "scalar"  << "substract" ;
    d->types["Scalar Array Substract"] = "array_scalar_substract";

    d->nodes << "Scalar Array Mult";
    d->descriptions["Scalar Array Mult"] = "<p>Description not yet filled!</p>";
    d->tags["Scalar Array Mult"] = QStringList() << "container" << "array" << "scalar"  << "mult" ;
    d->types["Scalar Array Mult"] = "array_scalar_mult" ;

    d->nodes << "Scalar Array Divide";
    d->descriptions["Scalar Array Divide"] = "<p>Description not yet filled!</p>";
    d->tags["Scalar Array Divide"] = QStringList() << "container" << "array" << "scalar"  << "divide" ;
    d->types["Scalar Array Divide"] = "array_scalar_divide" ;
}

void dtkComposerFactory::initNodeContainerData(void)
{
    // d->nodes << "Data Container";
    // d->descriptions["Data Container"] = "<p>Description not yet filled!</p>";
    // d->tags["Data Container"] = QStringList() << "container" << "data";
    // d->types["Data Container"] = "data_container";

    // d->nodes << "Data Array";
    // d->descriptions["Data Array"] = "<p>Description not yet filled!</p>";
    // d->tags["Data Array"] = QStringList() << "container" << "array" << "data" ;
    // d->types["Data Array"] = "array_data";

    // d->nodes << "Data Array Extractor";
    // d->descriptions["Data Array Extractor"] = "<p>Description not yet filled!</p>";
    // d->tags["Data Array Extractor"] = QStringList() << "container" << "array" << "data" << "extractor";
    // d->types["Data Array Extractor"] = "array_data_extractor";

    // d->nodes << "Data Array SubArray";
    // d->descriptions["Data Array SubArray"] = "<p>Description not yet filled!</p>";
    // d->tags["Data Array SubArray"] = QStringList() << "container" << "data" << "array" << "subarray"<< "extractor";
    // d->types["Data Array SubArray"] = "array_data_extractor_subarray";

    // d->nodes << "Data Array Part";
    // d->descriptions["Data Array Part"] = "<p>Description not yet filled!</p>";
    // d->tags["Data Array Part"] = QStringList() << "container" << "data" << "array" << "part" << "subarray" << "extractor";
    // d->types["Data Array Part"] = "array_data_extractor_array_part";

    // d->nodes << "Data Array Insert";
    // d->descriptions["Data Array Insert"] = "<p>Description not yet filled!</p>";
    // d->tags["Data Array Insert"] = QStringList() << "container" << "array" << "data"  << "insert" ;
    // d->types["Data Array Insert"] = "array_data_insert";

    // d->nodes << "Data Array Set";
    // d->descriptions["Data Array Set"] = "<p>Description not yet filled!</p>";
    // d->tags["Data Array Set"] = QStringList() << "container" << "array" << "data"  << "set";
    // d->types["Data Array Set"] = "array_data_set";

    // d->nodes << "Data Array Append";
    // d->descriptions["Data Array Append"] = "<p>Description not yet filled!</p>";
    // d->tags["Data Array Append"] = QStringList() << "container" << "array" << "data"  << "append";
    // d->types["Data Array Append"] = "array_data_append";

    // d->nodes << "Data Array Prepend";
    // d->descriptions["Data Array Prepend"] = "<p>Description not yet filled!</p>";
    // d->tags["Data Array Prepend"] = QStringList() << "container" << "array" << "data"  << "prepend";
    // d->types["Data Array Prepend"] = "array_data_prepend";
}

void dtkComposerFactory::initNodeControl(void)
{
    d->nodes << "Case";
    d->descriptions["Case"] = dtkReadFile(":dtkComposer/dtkComposerNodeControlCase.html");
    d->tags["Case"] = QStringList() << "control" << "case";
    d->types["Case"] = "case";

    d->nodes << "Do While";
    d->descriptions["Do While"] = "<p>Description not yet filled!</p>";
    d->tags["Do While"] = QStringList() << "control" << "do" << "while";
    d->types["Do While"] = "do while";

    d->nodes << "If";
    d->descriptions["If"] = dtkReadFile(":dtkComposer/dtkComposerNodeControlIf.html");
    d->tags["If"] = QStringList() << "control" << "if";
    d->types["If"] = "if";

    d->nodes << "For";
    d->descriptions["For"] = "<p>Description not yet filled!</p>";
    d->tags["For"] = QStringList() << "control" << "for";
    d->types["For"] = "for";

    d->nodes << "Foreach";
    d->descriptions["Foreach"] = "<p>Description not yet filled!</p>";
    d->tags["Foreach"] = QStringList() << "control" << "foreach";
    d->types["Foreach"] = "foreach";

    d->nodes << "Map";
    d->descriptions["Map"] = "<p>High order function: takes a container and applies a set of functions to each of its element to generate a new container</p>";
    d->tags["Map"] = QStringList() << "control" << "map" << "container";
    d->types["Map"] = "map";

    d->nodes << "While";
    d->descriptions["While"] = "<p>Description not yet filled!</p>";
    d->tags["While"] = QStringList() << "control" << "while";
    d->types["While"] = "while";
}

dtkComposerFactory::~dtkComposerFactory(void)
{
    delete d;

    d = NULL;
}

dtkComposerNode *dtkComposerFactory::create(const QString& type)
{
    if(type == "composite")
        return new dtkComposerNodeComposite;

    // constant nodes

    if(type == "pi")
        return new dtkComposerNodePi;

    if(type == "e")
        return new dtkComposerNodeE;

    // primitive nodes

    if(type == "boolean")
        return new dtkComposerNodeBoolean;

    if(type == "integer")
        return new dtkComposerNodeInteger;

    if(type == "real")
        return new dtkComposerNodeReal;

    if(type == "string")
        return new dtkComposerNodeString;

    // if(type == "file")
    //     return new dtkComposerNodeFile;

    // container nodes

    // if(type == "data_container")
    //     return new dtkComposerNodeContainerData;

    // if(type == "array_data")
    //     return new dtkComposerNodeArrayData;

    // if(type == "array_data_extractor")
    //     return new dtkComposerNodeArrayDataExtractor;

    // if(type == "array_data_extractor_subarray")
    //     return new dtkComposerNodeArrayDataExtractorSubArray;

    // if(type == "array_data_extractor_array_part")
    //     return new dtkComposerNodeArrayDataExtractorArrayPart;

    // if(type == "array_data_insert")
    //     return new dtkComposerNodeArrayDataOperatorInsert;

    // if(type == "array_data_set" || type == "array_data_replace")
    //     return new dtkComposerNodeArrayDataOperatorSet;

    // if(type == "array_data_append")
    //     return new dtkComposerNodeArrayDataOperatorAppend;

    // if(type == "array_data_prepend")
    //     return new dtkComposerNodeArrayDataOperatorPrepend;

    // // Matrix Square Nodes

    // if(type == "matrix_square_real")
    //     return new dtkComposerNodeMatrixSquareReal;

    // if(type == "matrixSquare_real_extractor")
    //     return new dtkComposerNodeMatrixSquareRealExtractor;

    // if(type == "matrixSquare_real_transpose")
    //     return new dtkComposerNodeMatrixSquareRealOperatorUnaryTranspose;

    // if(type == "matrixSquare_real_inverse")
    //     return new dtkComposerNodeMatrixSquareRealOperatorUnaryInverse;

    // if(type == "matrixSquare_real_trace")
    //     return new dtkComposerNodeMatrixSquareRealOperatorUnaryScalarTrace;

    // if(type == "matrixSquare_real_sum")
    //     return new dtkComposerNodeMatrixSquareRealOperatorBinarySum;

    // if(type == "matrixSquare_real_substract")
    //     return new dtkComposerNodeMatrixSquareRealOperatorBinarySubstract;

    // if(type == "matrixSquare_real_mult")
    //     return new dtkComposerNodeMatrixSquareRealOperatorBinaryMult;

    // if(type == "matrixSquare_real_ProductMatrixVector")
    //     return new dtkComposerNodeMatrixSquareRealOperatorBinaryRightProductMV;

    // if(type == "matrixSquare_real_ProductVectorMatrix")
    //     return new dtkComposerNodeMatrixSquareRealOperatorBinaryLeftProductVM;

    // if(type == "matrixSquare_real_ReplaceRowMatrixByVector")
    //     return new dtkComposerNodeMatrixSquareRealOperatorBinaryReplaceRowMatrixByVector;

    // if(type == "matrixSquare_real_ReplaceColMatrixByVector")
    //     return new dtkComposerNodeMatrixSquareRealOperatorBinaryReplaceColMatrixByVector;

    // Vector Real nodes

    // if(type == "vector_real" || type == "vectorReal")
    //     return new dtkComposerNodeVectorReal;

    // if(type == "vector_real_extractor")
    //     return new dtkComposerNodeVectorRealExtractor;

    // if(type == "vector_real_set")
    //     return new dtkComposerNodeVectorRealOperatorModifierSet;

    // if(type == "vector_real_sum")
    //     return new dtkComposerNodeVectorRealOperatorModifierSum;

    // if(type == "vector_real_substract")
    //     return new dtkComposerNodeVectorRealOperatorModifierSubstract;

    // if(type == "vector_real_mult")
    //     return new dtkComposerNodeVectorRealOperatorModifierMult;

    // if(type == "vector_real_divide")
    //     return new dtkComposerNodeVectorRealOperatorModifierDivide;

    // if(type == "vector_real_add_all")
    //     return new dtkComposerNodeVectorRealOperatorModifierAllAdd;

    // if(type == "vector_real_substract_all")
    //     return new dtkComposerNodeVectorRealOperatorModifierAllSubstract;

    // if(type == "vector_real_mult_all")
    //     return new dtkComposerNodeVectorRealOperatorModifierAllMult;

    // if(type == "vector_real_divide_all")
    //     return new dtkComposerNodeVectorRealOperatorModifierAllDivide;

    // if(type == "vectorReal_unit")
    //     return new dtkComposerNodeVectorRealOperatorUnaryUnitary;

    // if(type == "vectorReal_norm")
    //     return new dtkComposerNodeVectorRealOperatorUnaryScalarNorm;

    // if(type == "vectorReal_sum")
    //     return new dtkComposerNodeVectorRealOperatorBinarySum;

    // if(type == "vectorReal_substract")
    //     return new dtkComposerNodeVectorRealOperatorBinarySubstract;

    // if(type == "vectorReal_dot_prod")
    //     return new dtkComposerNodeVectorRealOperatorBinaryScalarDotProd;

    // if(type == "vectorReal_scal_mult")
    //     return new dtkComposerNodeVectorRealOperatorHomotheticMult;

    // if(type == "vectorReal_scal_divide")
    //     return new dtkComposerNodeVectorRealOperatorHomotheticDivision;

    // // Array nodes

    if(type == "array_scalar")
        return new dtkComposerNodeArrayScalar;

    if(type == "array_scalar_extractor")
        return new dtkComposerNodeArrayScalarExtractor;

    if(type == "array_scalar_extractor_subarray")
        return new dtkComposerNodeArrayScalarExtractorSubArray;

    if(type == "array_scalar_extractor_array_part")
        return new dtkComposerNodeArrayScalarExtractorArrayPart;

    if(type == "array_scalar_insert")
        return new dtkComposerNodeArrayScalarOperatorInsert;

    if(type == "array_scalar_set" || type == "array_scalar_replace")
        return new dtkComposerNodeArrayScalarOperatorSet;

    if(type == "array_scalar_append")
        return new dtkComposerNodeArrayScalarOperatorAppend;

    if(type == "array_scalar_prepend")
        return new dtkComposerNodeArrayScalarOperatorPrepend;

    if(type == "array_scalar_all_add")
        return new dtkComposerNodeArrayScalarOperatorModifierAllAdd;

    if(type == "array_scalar_all_substract")
        return new dtkComposerNodeArrayScalarOperatorModifierAllSubstract;

    if(type == "array_scalar_all_mult")
        return new dtkComposerNodeArrayScalarOperatorModifierAllMult;

    if(type == "array_scalar_all_divide")
        return new dtkComposerNodeArrayScalarOperatorModifierAllDivide;

    if(type == "array_scalar_sum")
        return new dtkComposerNodeArrayScalarOperatorSum;

    if(type == "array_scalar_substract")
        return new dtkComposerNodeArrayScalarOperatorSubstract;

    if(type == "array_scalar_mult")
        return new dtkComposerNodeArrayScalarOperatorMult;

    if(type == "array_scalar_divide")
        return new dtkComposerNodeArrayScalarOperatorDivide;

    // algebraic nodes

    if(type == "vector3D")
        return new dtkComposerNodeVector3D;

    if(type == "vector3D_unit")
        return new dtkComposerNodeVector3DOperatorUnaryUnitary;

    if(type == "vector3D_norm")
        return new dtkComposerNodeVector3DOperatorUnaryScalarNorm;

    if(type == "vector3D_sum")
        return new dtkComposerNodeVector3DOperatorBinarySum;

    if(type == "vector3D_substract")
        return new dtkComposerNodeVector3DOperatorBinarySubstract;

    if(type == "vector3D_cross_prod")
        return new dtkComposerNodeVector3DOperatorBinaryCrossProd;

    if(type == "vector3D_dot_prod")
        return new dtkComposerNodeVector3DOperatorBinaryScalarDotProd;

    if(type == "vector3D_scal_mult")
        return new dtkComposerNodeVector3DOperatorHomotheticMult;

    if(type == "vector3D_scal_divide")
        return new dtkComposerNodeVector3DOperatorHomotheticDivision;

    if(type == "vector3D_triple_prod")
        return new dtkComposerNodeVector3DOperatorTernaryTripleProd;

    if(type == "vector3D_mixed_prod")
        return new dtkComposerNodeVector3DOperatorTernaryScalarMixedProd;

    if(type == "quaternion")
        return new dtkComposerNodeQuaternion;

    // if(type == "quat_unit")
    //     return new dtkComposerNodeQuaternionOperatorUnaryUnitary;

    // if(type == "quat_norm")
    //     return new dtkComposerNodeQuaternionOperatorUnaryScalarNorm;

    // if(type == "quat_sum")
    //     return new dtkComposerNodeQuaternionOperatorBinarySum;

    // if(type == "quat_substract")
    //     return new dtkComposerNodeQuaternionOperatorBinarySubstract;

    // if(type == "quat_scal_mult")
    //     return new dtkComposerNodeQuaternionOperatorHomotheticMult;

    // if(type == "quat_scal_divide")
    //     return new dtkComposerNodeQuaternionOperatorHomotheticDivision;

    // operator nodes

    if(type == "not")
        return new dtkComposerNodeBooleanOperatorUnaryNot;

    if(type == "and")
        return new dtkComposerNodeBooleanOperatorBinaryAnd;

    if(type == "or")
        return new dtkComposerNodeBooleanOperatorBinaryOr;

    if(type == "xor")
        return new dtkComposerNodeBooleanOperatorBinaryXor;

    if(type == "nand")
        return new dtkComposerNodeBooleanOperatorBinaryNand;

    if(type == "nor")
        return new dtkComposerNodeBooleanOperatorBinaryNor;

    if(type == "xnor")
        return new dtkComposerNodeBooleanOperatorBinaryXnor;

    if(type == "imp")
        return new dtkComposerNodeBooleanOperatorBinaryImp;

    if(type == "nimp")
        return new dtkComposerNodeBooleanOperatorBinaryNimp;

    if(type == "abs")
        return new dtkComposerNodeNumberOperatorUnaryAbs;

    if(type == "acos")
        return new dtkComposerNodeNumberOperatorUnaryAcos;

    if(type == "asin")
        return new dtkComposerNodeNumberOperatorUnaryAsin;

    if(type == "atan")
        return new dtkComposerNodeNumberOperatorUnaryAtan;

    if (type == "append")
        return new dtkComposerNodeStringOperatorBinaryAppend;

    if (type == "string_equality")
        return new dtkComposerNodeStringOperatorBinaryLogicEquality;

    if (type =="ceil")
        return new dtkComposerNodeNumberOperatorUnaryCeil;

    if (type =="cos")
        return new dtkComposerNodeNumberOperatorUnaryCos;

    if (type =="decr")
        return new dtkComposerNodeNumberOperatorUnaryDecr;

    if (type =="deg2rad")
        return new dtkComposerNodeNumberOperatorUnaryDeg2Rad;

    if (type =="exp")
        return new dtkComposerNodeNumberOperatorUnaryExp;

    if (type =="floor")
        return new dtkComposerNodeNumberOperatorUnaryFloor;

    if (type =="incr")
        return new dtkComposerNodeNumberOperatorUnaryIncr;

    if (type =="inv")
        return new dtkComposerNodeNumberOperatorUnaryInv;

    if (type =="ln")
        return new dtkComposerNodeNumberOperatorUnaryLn;

    if (type =="log10")
        return new dtkComposerNodeNumberOperatorUnaryLog10;

    if (type =="opp")
        return new dtkComposerNodeNumberOperatorUnaryOpp;

    if (type =="rad2deg")
        return new dtkComposerNodeNumberOperatorUnaryRad2Deg;

    if (type =="round")
        return new dtkComposerNodeNumberOperatorUnaryRound;

    if (type =="sin")
        return new dtkComposerNodeNumberOperatorUnarySin;

    if (type =="square")
        return new dtkComposerNodeNumberOperatorUnarySquare;

    if (type =="sqrt")
        return new dtkComposerNodeNumberOperatorUnarySqrt;

    if (type =="tan")
        return new dtkComposerNodeNumberOperatorUnaryTan;

    if (type =="eucldiv")
        return new dtkComposerNodeNumberOperatorBinaryEucldiv;

    if (type =="expn")
        return new dtkComposerNodeNumberOperatorBinaryExpn;

    if (type =="logn")
        return new dtkComposerNodeNumberOperatorBinaryLogn;

    if (type =="max")
        return new dtkComposerNodeNumberOperatorBinaryMax;

    if (type =="min")
        return new dtkComposerNodeNumberOperatorBinaryMin;

    if (type =="minus")
        return new dtkComposerNodeNumberOperatorBinaryMinus;

    if (type =="modulo")
        return new dtkComposerNodeNumberOperatorBinaryModulo;

    if (type =="mult")
        return new dtkComposerNodeNumberOperatorBinaryMult;

    if (type =="plus")
        return new dtkComposerNodeNumberOperatorBinaryPlus;

    if (type =="posnthroot")
        return new dtkComposerNodeNumberOperatorBinaryPosnthroot;

    if (type =="power")
        return new dtkComposerNodeNumberOperatorBinaryPower;

    if (type =="ratio")
        return new dtkComposerNodeNumberOperatorBinaryRatio;

    if (type =="equal")
        return new dtkComposerNodeNumberComparatorEqual;

    if (type =="notequal")
        return new dtkComposerNodeNumberComparatorNotequal;

    if (type =="gt")
        return new dtkComposerNodeNumberComparatorGt;

    if (type =="lt")
        return new dtkComposerNodeNumberComparatorLt;

    if (type =="gte")
        return new dtkComposerNodeNumberComparatorGte;

    if (type =="lte")
        return new dtkComposerNodeNumberComparatorLte;

    if (type =="almosteq")
        return new dtkComposerNodeNumberAlmosteq;

    if (type =="notalmosteq")
        return new dtkComposerNodeNumberNotalmosteq;

    // control nodes

    if(type == "case")
        return new dtkComposerNodeControlCase;

    if(type == "do while")
        return new dtkComposerNodeControlDoWhile;

    if(type == "if")
        return new dtkComposerNodeControlIf;

    if(type == "for")
        return new dtkComposerNodeControlFor;

    if(type == "foreach")
        return new dtkComposerNodeControlForEach;

    if(type == "map")
        return new dtkComposerNodeControlMap;

    if(type == "while")
        return new dtkComposerNodeControlWhile;

    // logger nodes

    if(type == "logger")
        return new dtkComposerNodeLogger;

//     // generic nodes

//     if(type == "data")
//         return new dtkComposerNodeData;

//     if(type == "process")
//         return new dtkComposerNodeProcess;

//     if(type == "view")
//         return new dtkComposerNodeView;

//     // distributed nodes

//     if(type == "remote")
//         return new dtkComposerNodeRemote;

//     if(type == "remoteSubmit")
//         return new dtkComposerNodeRemoteSubmit;

//     // /////////////////////////////////////////////////////////////////
//     // NITE nodes
//     // /////////////////////////////////////////////////////////////////

// #if defined(DTK_HAVE_NITE)
//     if(type == "kinectTracker")
//         return new dtkComposerNodeTrackerKinect;
// #endif

//     // /////////////////////////////////////////////////////////////////
//     // VRPN nodes
//     // /////////////////////////////////////////////////////////////////

// #if defined(DTK_HAVE_VRPN)
//     if(type == "vrpnTracker")
//         return new dtkComposerNodeTrackerVrpn;
// #endif

//     // /////////////////////////////////////////////////////////////////
//     // Plot nodes
//     // /////////////////////////////////////////////////////////////////

// #if defined(DTK_HAVE_PLOT)
//     if(type == "dtkPlotCurve")
//         return new dtkComposerNodePlotCurve;

//     if(type == "dtkPlotView")
//         return new dtkComposerNodePlotView;
// #endif

//     // /////////////////////////////////////////////////////////////////
//     // MPI nodes
//     // /////////////////////////////////////////////////////////////////

// #if defined(DTK_HAVE_MPI)
//     if(type == "world")
//         return new dtkComposerNodeWorld;

//     if(type == "communicatorSize")
//         return new dtkComposerNodeCommunicatorSize;

//     if(type == "communicatorRank")
//         return new dtkComposerNodeCommunicatorRank;

//     if(type == "communicatorUninitialize")
//         return new dtkComposerNodeCommunicatorUninitialize;

//     if(type == "communicatorInit")
//         return new dtkComposerNodeCommunicatorInit;

//     if(type == "communicatorSendInteger")
//         return new dtkComposerNodeCommunicatorSendInteger;

//     if(type == "communicatorReceiveInteger")
//         return new dtkComposerNodeCommunicatorReceiveInteger;

//     if(type == "communicatorSendReal")
//         return new dtkComposerNodeCommunicatorSendReal;

//     if(type == "communicatorReceiveReal")
//         return new dtkComposerNodeCommunicatorReceiveReal;

//     if(type == "communicatorSend")
//         return new dtkComposerNodeCommunicatorSend;

//     if(type == "communicatorReceive")
//         return new dtkComposerNodeCommunicatorReceive;
// #endif

    return NULL;
}

QList<QString> dtkComposerFactory::nodes(void)
{
    return d->nodes;
}

QHash<QString, QString> dtkComposerFactory::descriptions(void)
{
    return d->descriptions;
}

QHash<QString, QStringList> dtkComposerFactory::tags(void)
{
    return d->tags;
}

QHash<QString, QString> dtkComposerFactory::types(void)
{
    return d->types;
}
