from fastapi import FastAPI, Request
import subprocess
import os

app = FastAPI()

COMPILER_PATH = "./main"
NIKA_EXTENSION = ".nika"

@app.post("/execute")
async def execute(request: Request):
    binaryCode = await request.body()
    code = binaryCode.decode().replace("\"", '\\"')
    try:
        command = f'{COMPILER_PATH} "{code}"'
        os.system("ls -la")
        print(command)
        resp = subprocess.run(command, capture_output=True, shell=True)
        return resp
    except Exception as e:
        return {"error": str(e)}