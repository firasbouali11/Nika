import { useState } from "react";
import "./App.css"
import { Textarea, Card, Button, CardContent, Grid, Typography, Box } from '@mui/joy';
import Icon from '@mdi/react';
import { mdiPlay } from '@mdi/js';

function App() {

  const [code, setCode] = useState('');
  const [codeExecution, setCodeExecution] = useState('');

  const handleKeyDown = (event) => {
    if (event.key === 'Tab') {
      event.preventDefault();
      const { selectionStart, selectionEnd } = event.target;
      setCode(code.substring(0, selectionStart) + "\t" + code.substring(selectionEnd));
      // Set the cursor position after the inserted tab
      // event.target.selectionStart = selectionStart + 1;
    }
  };

  const handleChange = (event) => {
    setCode(event.target.value);
  };

  const runCode = async (event) => {
    const reqOptions = {
      method: "post",
      body: code
    }
    const rawResp = await fetch("http://localhost:8000/execute", reqOptions) //TODO: put the endpoint to execute code
    let resp = null
    if(rawResp.ok){
      resp = await rawResp.json()
      if(resp.returncode === 0)
        setCodeExecution(resp.stdout)
      else
        setCodeExecution(resp.stderr)
    }
    else
      alert("Something went wrong !")
  }

  const textareaStyle = {backgroundColor: "black", fontSize: 25}
  const outputStyle = {backgroundColor: "black"}

  return (
    <Box style={{padding: "0", margin: "0" }}>
      <h1 style={{ textAlign: "center", color: "green" }}>Nika Online Editor</h1>
      <div id="run-code">
        <Button size="md" variant="soft" color="success" className="black-background" onClick={runCode}>
          <span color="inherit">Run code</span><Icon path={mdiPlay} size={1} />
        </Button>
      </div>
      <Grid container>
        <Grid md={8} xs={12} paddingLeft={2} paddingRight={1}>
          <Textarea
            name="code"
            id="code"
            style={textareaStyle}
            color="success"
            minRows={18}
            onKeyDown={handleKeyDown}
            onChange={handleChange}
            value={code}>
          </Textarea>
        </Grid>
        <Grid md={4} xs={12} paddingLeft={1} paddingRight={2}>
          <Card variant="soft" style={outputStyle} color="success">
            <CardContent>
              <Typography className="bold" color="inherit" level="title-md">Executed Code</Typography>
              <br />
              <Typography color="inherit" >$sh {'>'} nikac ~/test.nika</Typography>
              <br />
              <Typography color="inherit" >{codeExecution}</Typography>
            </CardContent>
          </Card>
        </Grid>
      </Grid>
      <footer>© Created by: Firas Bouali - 2024</footer>
    </Box>
  );
}

export default App;

