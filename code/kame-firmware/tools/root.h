#define rootMessage String message = "<!doctype html>\n<html>\n<head>\n<title>Kame Control</title>\n</head>\n<body>\nKame controls:<br>\n<table>\n<tr>\n<td align='center'><input id='turnLeft' type='button' value='&#8598;' onclick='move(3);' /></td>\n<td align='center'><input id='moveForward' type='button' value='&#8593;' onclick='move(1);' /></td>\n<td align='center'><input id='turnRight' type='button' value='&#8599;' onclick='move(4);' /></td>\n</tr><tr>\n<td align='center'><input id='turnLeftForward' type='button' value='&#8592;' onclick='move(3);' /></td>\n<td align='center'><input id='stop' type='button' value='o' onclick='move(5);' /></td>\n<td align='center'><input id='turnRightForward' type='button' value='&#8594;' onclick='move(4);' /></td>\n</tr>\n</table>\n</body>\n<script>\nfunction move(command) {\nvar http = new XMLHttpRequest();\nvar url = '/command';\nvar params = 'command=' + command;\nhttp.open('POST', url, true);\n\n// Send the proper header information along with the request\nhttp.setRequestHeader('Content-type', 'application/x-www-form-urlencoded');\n\nhttp.onreadystatechange = function() {//Call a function when the state changes.\nif(http.readyState == 4 && http.status == 200) {\nconsole.log(http.responseText);\n}\n}\nhttp.send(params);\n}\n</script>\n</html>\n";