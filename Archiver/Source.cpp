#include <string>
#include <vector>
#include <fstream>
#include <map>
#include <list>
#include <iostream>
#include <bitset>
#include "BinaryTree.h"
#include <cmath>
#pragma region Legacy

std::string ReadAllAsString(std::string path)
{
	std::ifstream ifs(path);
	std::string s;
	std::getline(ifs, s, (char)ifs.eof());
	ifs.close();
	return s;
}

std::vector<std::string> ReadAllStrings(std::string path)
{
	std::fstream reader(path);
	std::vector<std::string> allStrings;
	if (reader.is_open())
	{
		std::string nextString;
		while (!reader.eof())
		{
			std::getline(reader, nextString);
			allStrings.push_back(nextString);
			nextString.clear();
		}
	}
	else
	{
		allStrings.push_back("WRONG FILE PATH: " + path);
		std::cout << "Wrong path!";
	}
	return allStrings;
}

std::vector<std::string> SplitString(const std::string &str, char separator)
{
	std::vector<std::string> words;
	std::string word;
	for (auto ch : str)
	{
		if (ch == separator)
		{
			words.push_back(word);
			word.clear();
		}
		else
		{
			word.push_back(ch);
		}
	}
	if (word.length() > 0)
	{
		words.push_back(word);
	}
	return words;
}

void WriteAllStrings(std::string path, std::vector<std::string> text)
{
	std::ofstream writer(path);
	for (auto str : text)
	{
		writer << str;
	}
	writer.close();
}

std::map<char, size_t> AnalyzeSymbolsFrequency(std::vector<std::string> text)
{
	std::map<char, size_t> result;
	for (auto str : text)
	{
		for (char symbol : str)
		{
			result[symbol]++;
		}
	}
	return result;
}

std::map<char, size_t> AnalyzeSymbolsFrequency(std::string &text)
{
	std::map<char, size_t> result;
	for (char &symbol : text)
	{
		result[symbol]++;
	}
	return result;
}

void Print(std::list<BinaryTree> &trees)
{
	for (auto tree : trees)
	{
		std::cout << tree.GetRoot()->GetFrequency() << ' ';
	}
	std::cout << '\n';
}

BinaryTree CreateHaffamnsTree(const std::map<char, size_t> &symbolsData)
{
	std::list<BinaryTree> HaffmanTrees;
	for (auto pair : symbolsData)
	{
		HaffmanTrees.push_back(new Node(pair.first, pair.second));
	}
	while (HaffmanTrees.size() > 1)
	{
		HaffmanTrees.sort();
		BinaryTree &first = *HaffmanTrees.begin();
		BinaryTree &second = *std::next(HaffmanTrees.begin(), 1);
		BinaryTree merged = first + second;
		HaffmanTrees.pop_front();
		HaffmanTrees.pop_front();
		HaffmanTrees.push_back(merged);
	}
	return *(HaffmanTrees.begin()); //we always have only one tree in an array in the end.
}

void ReadCode(Node *node, std::map<char, std::string> &codeTable, std::string curCode)
{
	if (node->GetLeft())
	{
		ReadCode(node->GetLeft(), codeTable, curCode + "0");
		if (node->GetRight())
		{
			ReadCode(node->GetRight(), codeTable, curCode + "1");
		}
	}
	else
	{
		codeTable.insert(std::pair<char, std::string>(node->GetSymbol(), curCode));
	}
}

std::map<char, std::string> CreateCodeFromTree(BinaryTree &codeTree)
{
	std::map<char, std::string> codeTable;
	ReadCode(codeTree.GetRoot(), codeTable, "");
	return codeTable;
}

unsigned char ConvertStringToByte(std::string str)
{
	unsigned char byte = 0;
	for (size_t i = 8 - str.size(); i < 8; i++)
	{
		if (str[7 - i] == '1')
		{
			byte = byte | (1 << i);
		}
	}
	return byte;
}

std::string ConvertByteToString(unsigned char byte)
{
	std::string str = "";
	for (size_t i = 0; i < 8; i++)
	{
		str.push_back((((1 <<(7 - i)) & byte) ? '1' : '0'));
	}
	return str;
}

std::map<std::string, char> CreateTable(std::string tableAsString)
{
	std::map<std::string, char> result;
	std::string code = "";
	for (size_t i = 0; i < tableAsString.length(); i++)
	{
		if (tableAsString[i] == ' ')
		{
			result.insert(std::make_pair(code, tableAsString[i + 1]));
			code.clear();
			i += 2;
		}
		else
		{
			code.push_back(tableAsString[i]);
		}
	}
	return result;
}


void Compress(std::string text, std::map<char, std::string> codeTable, std::string output)
{
	std::ofstream writer(output);
	std::ofstream tableWriter(output + ".enc");
	for (auto code : codeTable)
	{
		tableWriter << code.second << ' ' << code.first << '\n';
	}
	std::string byteAsString = "";
	for (auto symbol : text)
	{
		std::string code = codeTable[symbol];
		for (size_t i = 0; i < code.size(); i++)
		{
			byteAsString.push_back(code[i]);
			if (byteAsString.size() == 8)
			{
				writer << ConvertStringToByte(byteAsString);
				byteAsString.clear();
			}
		}
	}
	writer.close();
	tableWriter.close();
}


std::string Decompress(std::string path)
{
	auto encodeTable = CreateTable(path + ".enc");
	std::string text = "";
	std::string curentCode = "";
	std::ifstream reader(path);
	unsigned char byte;
	auto WTF = ReadAllAsString(path);
	while(!reader.eof())
	{
		reader >> byte;
		std::string bits = ConvertByteToString(byte);
		for (size_t i = 0; i < 8; i++)
		{
			curentCode.push_back(bits[i]);
			if (encodeTable.find(curentCode) != encodeTable.end())
			{
				text.push_back(encodeTable[curentCode]);
				curentCode.clear();
			}
		}
	}
	return text;
}

#pragma endregion

// write
void Write(std::string path, std::bitset<8> byte)
{
	std::ofstream output(path, std::ios::binary);
	unsigned long n = byte.to_ulong();
	output.write(reinterpret_cast<const char*>(&n), sizeof(n));
}

void TestCompress(std::string text, std::map<char, std::string> codeTable, std::string output)
{
	std::ofstream writer(output, std::ios::binary);
	std::string byteAsString = "";
	for (auto symbol : text)
	{
		std::string code = codeTable[symbol];
		for (size_t i = 0; i < code.size(); i++)
		{
			byteAsString.push_back(code[i]);
			if (byteAsString.size() == 8)
			{
				char code = ConvertStringToByte(byteAsString);
				writer.write(reinterpret_cast<const char*>(&code), sizeof(code));
				byteAsString.clear();
			}
		}
	}
	if (byteAsString.size() > 0)
	{
		char code = ConvertStringToByte(byteAsString);
		writer.write(reinterpret_cast<const char*>(&code), sizeof(code));
	}
	std::ofstream tableWriter(output + ".enc");
	tableWriter << (8 - byteAsString.size()) << '\n';
	for (auto code : codeTable)
	{
		tableWriter << code.second << ' ' << code.first << '\n';
	}
}

std::vector<unsigned char> Read(std::string path)
{
	std::ifstream input(path, std::ios::binary);
	std::vector<unsigned char> text;
	unsigned char byte;
	while (!input.eof())
	{
		input >> byte;
		text.push_back(byte);
	}
	//std::cout << (int)text[text.size() - 3] << ' ' << (int)text[text.size() - 2] << ' ' << (int)text[text.size() - 1] << '\n';
	return text;
}
std::string GetBits(std::vector<unsigned char> bytes)
{
	std::string bits = "";
	for (auto byte : bytes)
		bits = bits + std::bitset<8>(byte).to_string();
	return bits;
}
std::string TestDecompress(std::string path)
{
	auto table = ReadAllAsString(path + ".enc");
	int zeroesCount = (table[0] - '0');
	table = table.substr(2, table.size() - 1);
	auto encodeTable = CreateTable(table);
	std::string text = "";
	std::string curentCode = "";
	unsigned char byte;
	auto codedText = Read(path);
	auto bits = GetBits(codedText);
	for (size_t i = 0; i < bits.size() - zeroesCount - 1; i++)
	{
			curentCode.push_back(bits[i]);
			if (encodeTable.find(curentCode) != encodeTable.end())
			{
				text.push_back(encodeTable[curentCode]);
				curentCode.clear();
			}
	}
	return text;
}

int main()
{
	const std::string input = "input.txt";
	const std::string output = "output.txt";
	auto text = ReadAllAsString(input);
	auto frequency = AnalyzeSymbolsFrequency(text);
	BinaryTree codesTree = CreateHaffamnsTree(frequency);
	auto codeTable = CreateCodeFromTree(codesTree);

	//Old byte compressor

	//Compress(text, codeTable, output);
	//std::string decompText = Decompress(output);
	//std::ofstream A("decoded.txt");
	//A.write(decompText.c_str(), decompText.size());
	//A.close();
	//if (decompText == text)
	//{
	//	std::cout << "YAAAAAAAAAAY\n";
	//}
	//else
	//{
	//	std::cout << "TOTALY NOT YAAAAAAAAAAAAAAAY \n";
	//}

	//Non byte compressor

	//std::ofstream writer("decoded.txt");
	//NonByteCompress(text, codeTable, output);
	//auto decompText = NonByteDecompress(output);
	//writer.write(decompText.c_str(), decompText.size());

	std::ofstream writer("decoded.txt");
	TestCompress(text, codeTable, output);
	auto sec = Read(output);
	auto decompText = TestDecompress(output);
	//for (size_t i = 0; i < std::fmin(text.size(), decompText.size()); i++)
	//{
	//	if (text[i] != decompText[i])
	//	{
	//		std::cout << i << std::endl;
	//	}
	//}
	writer.write(decompText.c_str(), decompText.size());

	return 0;
}