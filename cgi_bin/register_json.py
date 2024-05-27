import json
from pymongo import MongoClient

# url = 'mongodb://root:klingon@192.168.1.20:27017'
url = 'mongodb://root:tomate@192.168.39.138:27017'

client = MongoClient(url)
db_name = 'prueba'

def create_user(user_data):
    try:
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
    import argparse

    parser = argparse.ArgumentParser(description='Process some JSON data.')
    parser.add_argument('json_data', type=str, help='JSON data as string')
    args = parser.parse_args()

    json_string = args.json_data
    user_data = json.loads(json_string)
    
    username = user_data.get('username')

    user_id = create_user(user_data)
    while user_id is None:
        user_id = create_user(user_data)
    
    with open("./conf_web/.output.html", "w") as f:
        f.write(doc(username))

# "{\"username\":\"nombre\",\"password\":\"121\",\"email\":\"swsw@sw.ws\"}"