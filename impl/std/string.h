#ifdef _CCTYPE_ //<cctype>
bool isspace(char s) { return std::isspace(static_cast<unsigned char>(s)); }

std::string left_trim(const std::string& s) {
	size_t start = 0;
	while (start < s.size() && cyx::isspace(s[start])) { ++start; }
	return s.substr(start);
}

std::string right_trim(const std::string& s) {
	size_t end = s.size();
	while (end > 0 && cyx::isspace(s[end - 1])) { --end; }
	return s.substr(0, end);
}

std::string trim(const std::string& s) {
	size_t start = 0, end = s.size();
	while (start < end && cyx::isspace(s[start])) { ++start; }
	while (end > start && cyx::isspace(s[end - 1])) { --end; }
	return s.substr(start, end - start);
}
#endif //_CCTYPE_



#ifdef _CWCTYPE_ //<cwctype>
std::wstring left_trim(const std::wstring& s) {
	size_t start = 0;
	while (start < s.size() && std::iswspace(s[start])) { ++start; }
	return s.substr(start);
}

std::wstring right_trim(const std::wstring& s) {
	size_t end = s.size();
	while (end > 0 && std::iswspace(s[end - 1])) { --end; }
	return s.substr(0, end);
}

std::wstring trim(const std::wstring& s) {
	size_t start = 0, end = s.size();
	while (start < end && std::iswspace(s[start])) { ++start; }
	while (end > start && std::iswspace(s[end - 1])) { --end; }
	return s.substr(start, end - start);
}
#endif //_CWCTYPE_



#ifdef _STRING_ //<string>

void getline_cond(std::istream& is, std::string& str,
				  bool (*cond)(const std::string&) =
				  [](const std::string& str)->bool { return str != ""; }) {
	while (true) {
		std::getline(is, str);
		if (cond(str)) { return; }
	}
}

#endif //_STRING_