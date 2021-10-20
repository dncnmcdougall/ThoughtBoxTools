#include "Arg.h"

bool Arg::shouldActivate(const char* arg) const 
{ 
    return m_longName == arg || m_shortName == arg; 
};

Arg::status Arg::consume(const char* value) 
{
    if ( m_begin ) 
    {
        if ( m_unique && m_collectedParams.size() != 0 ) {
            m_begin = true;
            return ABORT;
        }
        m_collectedParams.push_back(std::vector<std::string>());
        m_begin = false;
    } 
    m_collectedParams.back().push_back(value);
    if ( m_collectedParams.back().size() == m_params+1 ) 
    {
        m_collectedParams.back().erase(m_collectedParams.back().begin());
        m_begin = true;
        return COMPLETE;
    } else {
        return CONTINUE;
    }
}

unsigned long int Arg::occurences() const
{
    return m_collectedParams.size();
}

std::string Arg::description() const
{
    std::string result = m_shortName+" "+ m_longName+"\t"+m_desc;
    return result;
}
