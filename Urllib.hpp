#include<locale>
#include<map>
#include<sstream>
#include<algorithm>        

namespace Urllib{

class EncodeDecodeBase{

protected:
    std::map <std::string, std::string> _parameters;
    // Eg: http://example.com
    std::string _baseUrl;
    // Eg: http://example.com?key1=val1&key2=val2
    std::string _url;

public:

    EncodeDecodeBase(){}

    virtual void setParameter(const std::pair<std::string, std::string> &iPair){
        this->setParameter(iPair.first, iPair.second);
    }

    virtual void setParameter(const std::string& iKey, const std::string& iValue){
       _parameters[iKey] = iValue;
    }

    virtual void clear(){
        _baseUrl.clear();
        _parameters.clear();
        _url.clear();
    }

    virtual size_t removeParameter(const std::string& iKey){
        return _parameters.erase(iKey);
    }

    virtual void setBaseUrl(const std::string &iBaseUrl){
        _baseUrl = iBaseUrl;
    }

    virtual const std::string& getBaseUrl(){
        return _baseUrl;
    }

    virtual void setUrl(const std::string &iUrl){
        _url = iUrl;
    }

    virtual const std::string& getUrl(){
        return _url;
    }

    virtual const std::map<std::string, std::string>& getParameters(){
        return _parameters;
    }
};



class Encode : public EncodeDecodeBase{

    std::string encode(const std::string &aString){
        std::string aEncodedString = "";
        for(char aChar : aString){
            if(!std::isalnum(aChar) && aChar != '-' && aChar != '_' && aChar !='.' && aChar != '~'){
                // Hex encode
                aEncodedString += "%";
                std::stringstream ss;
                ss << std::hex << (int) aChar;
                std::string aTempStr = ss.str();
                /* Apparently two overloaded toupper methods exist.
                * One in the std namespace and one in the global namespace
                * Good going C++, you god damned cryptic esoteric parseltongue
                */
                std::transform(aTempStr.begin(), aTempStr.end(), aTempStr.begin(), ::toupper);
                aEncodedString += aTempStr;
            }
            else{
                //Otherwise, keep it as it is
                aEncodedString += aChar;
            }
        }
        return aEncodedString;
    }

    std::string encode(const std::pair<std::string, std::string> &iPair){
        std::string aEscapedString = "";
        aEscapedString += encode(iPair.first);
        aEscapedString += "=";
        aEscapedString += encode(iPair.second);
        aEscapedString += "&";
        return aEscapedString;
    }

public:

    Encode(){}

    std::string encode(){
        std::string _url = _baseUrl + "?";
        for(auto &iPair : _parameters){
            _url += encode(iPair);
        }
        // Remove the extra '&'
        _url.erase(_url.end()-1);
        return _url;
    }
}; // class Encode


class Decode : public EncodeDecodeBase{

    std::vector<std::string> split(const std::string &iString, const char iDelimiter){
        size_t tokenStartPos = 0;
        size_t posOfDelimiter = iString.find_first_of(iDelimiter);
        std::vector<std::string> oTokens;
        //If we have n delims, then there are n+1 tokens
        oTokens.reserve(std::count(iString.begin(), iString.end(), iDelimiter) + 1);
        while(std::string::npos != posOfDelimiter){
            oTokens.push_back(iString.substr(tokenStartPos, posOfDelimiter));
            //For the next loop
            tokenStartPos = posOfDelimiter+1;
            posOfDelimiter = iString.find_first_of(iDelimiter, tokenStartPos);
        }
        //There is one more (last )token left to process
        oTokens.push_back(iString.substr(tokenStartPos));
        oTokens.shrink_to_fit();
        return oTokens;
    }

    void buildParameters(const std::string &iParameters){
        auto keyValTokens = split(iParameters, '&');
        for(auto keyValToken : keyValTokens){
            if(keyValToken.size() < 1){
                continue;
            }
            auto keyValVector = split(keyValToken, '=');
            if(keyValVector.size() != 2){
                continue;
            }
            _parameters[keyValVector[0]] = keyValVector[1];
        }
    }

    const std::string& decode(const std::string iEncodedUrl){
        size_t posOfQMark = iEncodedUrl.find_first_of('?');
        if(posOfQMark == std::string::npos){
            //Nothing to decode
            _url = iEncodedUrl;
            return _url;
        }
        //Everything prior to ? is the baseUrl
        _baseUrl = iEncodedUrl.substr(0, posOfQMark);
        //If '?' is at n, we need n+1 characters from the start, i.e including the ?.
        _url = iEncodedUrl.substr(0, posOfQMark+1);
        std::string parameters;
        for(size_t pos = posOfQMark+1; pos < iEncodedUrl.size(); ++pos){
            // Look for %XX
            if('%' == iEncodedUrl[pos]){
                if(pos+2 >= iEncodedUrl.size()){
                    //We have reached the end of the url. We cannot decode it because there is nothing to decode
                    break;
                }
                //Decode next two chars and increase the pos counter accordingly
                //Get the next two characters after % and store it in iss
                std::istringstream iss(iEncodedUrl.substr(pos+1, 2));
                int aIntVal;
                iss >> std::hex >> aIntVal;
                parameters += static_cast<char> (aIntVal);
                //We have processed two extra characters
                pos += 2;
            }
            else{
                //Nothing to do, just copy
                parameters += iEncodedUrl[pos];
            }
        }
        this->buildParameters(parameters);
        _url += parameters;
        return _url;
    }

public:

    Decode(){}

    const std::string& decode(){
        if(_url.empty()){
            return _url;
        }
        else{
            return this->decode(_url);
        }
    }

}; // class Decode

} // namespace Urllib

