import sys
from urllib.parse import parse_qs


def parse_query_string(query_string):
    parsed_dict = parse_qs(query_string)
    return {k: v[0] for k, v in parsed_dict.items()}


def createCookie(query_string):

	data = parse_query_string(query_string)

	key = data.get('key')
	value = data.get('value')

	cookie = key + '=' + '\"' + value + '\"'

	return cookie


if __name__ == "__main__":
	query_string = sys.argv[1]
	print(createCookie(query_string))

# "ket=NameKey&value=todo_esto_es_value"

# "cokie=\"todo esto es el varlo\""
