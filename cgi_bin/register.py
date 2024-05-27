import sys
from pymongo import MongoClient
from urllib.parse import parse_qs


url = 'mongodb://root:tomate@192.168.39.138:27017'
# url = 'mongodb://root:klingon@192.168.1.20:27017'
client = MongoClient(url)
db_name = 'prueba'

def parse_query_string(query_string):
    parsed_dict = parse_qs(query_string)
    return {k: v[0] for k, v in parsed_dict.items()}

def create_user(user_data):
    try:
        client = MongoClient(url)
        db = client[db_name]
        coleccion = db['users']
        result = coleccion.insert_one(user_data)
        return result.inserted_id
    except Exception as e:
        print("An error occurred:", e)
        return None
    finally:
        client.close()

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
        
        # # Eliminar 'confirmPassword' de user_data
        # if 'confirmPassword' in user_data:
        #     del user_data['confirmPassword']
        username = user_data.get('username')

        user_data_complete = {
            'username': user_data.get('username'),
            'email': user_data.get('email'),
            'password': user_data.get('password')
        }

        # print(user_data_complete)
     
        user_id = create_user(user_data_complete)
        while user_id is None:
            # Si la inserción no se ha completado, esperar un momento antes de intentarlo de nuevo
            user_id = create_user(user_data_complete)
        
        # Si la inserción se ha completado correctamente, escribir en el archivo HTML
        with open("./conf_web/.output.html", "w") as f:
            f.write(doc(username))

# "username=nacho&email=nacho.mota%40gmail.com&password=12345&confirmPassword=12345"
