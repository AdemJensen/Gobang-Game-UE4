#pragma once

#include <vector>
#include <algorithm>
#include <map>

template <typename T1, typename T2>
class KizunaAiCache
{
private:
	/*
	To judge if two vals stored in this cache is the same.
	Return true if two are same.
	*/
	bool (*judgeValSame)(T2, T2);

	std::map<T1, std::vector<T2> > store;
	std::vector<T1> keys;

	static bool defaultValSame(T2, T2)
	{
		return false;
	}

public:

	KizunaAiCache()
	{
		this->judgeValSame = defaultValSame;
	}

	KizunaAiCache(bool (*judgeValSameFunc)(T2, T2))
	{
		this->judgeValSame = judgeValSameFunc;
	}

	void setJudgeFunc(bool (*judgeValSameFunc)(T2, T2))
	{
		this->judgeValSame = judgeValSameFunc;
	}

	void clearVals()
	{
		for (int i = 0; i < keys.size(); i++)
		{
			store[keys[i]] = std::vector<T2>();
			/*std::vector<T2> checker = store[keys[i]];
			checker.clear();*/
		}
	}

	void addKey(T1 key)
	{
		if (std::find(keys.begin(), keys.end(), key) == keys.end())
		{
			keys.push_back(key);
			store[key] = std::vector<T2>();
		}
	}

	void addVal(T1 key, T2 val)
	{
		if (std::find(keys.begin(), keys.end(), key) != keys.end())
		{
			store[key].push_back(val);
		}
	}

	std::vector<T1> getKeySet()
	{
		std::vector<T1> result;
		for (int i = 0; i < keys.size(); i++)
		{
			result.push_back(keys[i]);
		}
		return result;
	}

	std::vector<T1> getRelativeKeySet(T2 val)
	{
		std::vector<T1> result;
		for (int i = 0; i < keys.size(); i++)
		{
			std::vector<T2> checker = store[keys[i]];
			for (int j = 0; j < checker.size(); j++)
			{
				if (judgeValSame(val, checker[j]))
				{
					result.push_back(keys[i]);
				}
			}
		}
		return result;
	}

	std::vector<T2> getValSet(T1 key)
	{
		std::vector<T2> result;
		std::vector<T2> source = store[key];
		for (int i = 0; i < source.size(); i++)
		{
			result.push_back(source[i]);
		}
		return result;
	}

	std::vector<T2> getAllElements()
	{
		std::vector<T2> result;
		for (int i = 0; i < keys.size(); i++)
		{
			std::vector<T2> checker = store[keys[i]];
			for (int j = 0; j < checker.size(); j++)
			{
				result.push_back(checker[j]);
			}
		}
		return result;
	}

};