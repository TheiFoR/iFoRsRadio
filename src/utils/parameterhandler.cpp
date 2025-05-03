#include "parameterhandler.h"


ParameterHandler::ParameterHandler(const QVariantMap &parameters)
    : m_parameters(parameters)
{
}

void ParameterHandler::setParameters(const QVariantMap &parameters)
{
    QVariantMap& param = const_cast<QVariantMap&>(m_parameters);
    param = parameters;
}
