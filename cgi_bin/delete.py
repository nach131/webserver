import os

def delete_file(filePath, realFileName):
	html = """
	<!DOCTYPE html>
	<html lang="en">
		<head>
			<meta charset="UTF-8">
			<title>Delete file</title>
		</head>
	<body>
	"""
	try:
		if os.path.exists(filePath):
			os.remove(filePath)
			result = f"File '{realFileName}' was deleted successfully"
		else:
			result = f"Error: File '{realFileName}' does not exits"
	except Exception as e:
		result = f"Error when trying to delete the file '{realFileName}': {str(e)}"
	html += """<center><h1>{}</h1></center>
			<hr>
			<center>vduchi & nmota-bu</center>
	</body>
		</html""".format(result)
	return html

if __name__ == "__main__":
	import sys
	if len(sys.argv) != 3:
		print("Usage: python script.py directory_path root_location")
	else:
		filePath = sys.argv[1]
		realFileName = sys.argv[2]
		print(delete_file(filePath, realFileName))