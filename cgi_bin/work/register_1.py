import sys
from urllib.parse import parse_qs
from pymongo import MongoClient
# from html import escape

def parse_query_string(query_string):
    # Parse the query string into a dictionary
    parsed_dict = {k: v[0] for k, v in parse_qs(query_string).items()}
    return parsed_dict

url = 'mongodb://root:klingon@192.168.1.20:27017'
client = MongoClient(url)
db_name = 'prueba'

def create_user(user_data):
    try:
        client = MongoClient(url)
        db = client[db_name]
        coleccion = db['users']
        result = coleccion.insert_one(user_data)
    except Exception as e:
        print("An error occurred:", e)
        return None
    finally:
        client.close()

    return result.inserted_id

def doc(username):
    html = """
    <!DOCTYPE html>
    <html lang="en">
        <head>
            <meta charset="UTF-8">
            <title>Create User</title>
        </head>
        <body>
            <center><h1>User {} was created</h1></center>
            <hr>
            <center>vduchi & nmota-bu</center>
        </body>
    </html>
    """.format(username)
    return html

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: python script.py datos")
    else:
        query_string = sys.argv[1]
        user_data = parse_query_string(query_string)
        
        # Eliminar 'confirmPassword' de user_data
        if 'confirmPassword' in user_data:
            del user_data['confirmPassword']
        if 'username' in user_data:
            userName = user_data['username']
     
        user_id = create_user(user_data)
            # print(f"User created with ID: {user_id}")
        if user_id:
            # Escribir en el archivo output.html
            with open("./conf_web/.output.html", "w") as f:
                f.write(doc(userName))
        else:
            print("Failed to create user")


# "username=nacho&email=nacho.mota%40gmail.com&password=12345&confirmPassword=12345"

    # """.format(escape(username))
