from fastapi import FastAPI, Request
import os 
import subprocess

from utils import generate_timeuuid

app = FastAPI()

TEST = True
COMPILER_PATH = "../Nika/main"
NIKA_EXTENSION = ".nika"

@app.post("/execute")
async def execute(request: Request):
    binaryCode = await request.body()
    code = binaryCode.decode().replace("\"", '\\"')
    try:
        command = f'{COMPILER_PATH} "{code}"'
        print(command)
        resp= subprocess.run(command, capture_output=True, shell=True)
        return resp
    except Exception as e:
        return {"error": str(e)}