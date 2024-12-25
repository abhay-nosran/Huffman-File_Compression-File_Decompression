const express = require('express')
const app = express() 
const { exec } = require('child_process')
const port = 8080 

const multer  = require('multer')
const { stdout, stderr } = require('process')
const upload = multer({ dest: './uploads/' })

const path = require('path');
const fs = require('fs');


app.get("/",(req,res)=>{
    res.sendFile(path.join(__dirname,"index.html"))
})
app.post('/compress',upload.single('txtFile'),(req,res) => {

    
    const inputFilePath = `${req.file.path}.txt` ;
    fs.rename(req.file.path,inputFilePath,(err) =>{
        if (err) {
            return res.status(500).send('Error saving the file');
          }
    }) ;
    const outputFilePath = `${req.file.path}.bin` ;

    console.log(inputFilePath) ;
    console.log(outputFilePath) ;

    

    exec(`compressor.exe ${inputFilePath} ${outputFilePath}`,function(err,stdout,stderr) {
        if (err) {
            console.error(`Error: ${stderr}`);
            return res.status(500).send('Compression failed.');
        }
        
        res.download(outputFilePath,"compressed.bin",function(){
            fs.unlinkSync(inputFilePath); 
            fs.unlinkSync(outputFilePath); 
        })

    })
})
app.post('/decompress',upload.single('binFile'),(req,res) => {

    
    const inputFilePath = `${req.file.path}.bin` ;
    fs.rename(req.file.path,inputFilePath,(err) =>{
        if (err) {
            return res.status(500).send('Error saving the file');
          }
    }) ;
    const outputFilePath = `${req.file.path}.txt` ;

    console.log(inputFilePath) ;
    console.log(outputFilePath) ;

    

    exec(`decompressor.exe ${inputFilePath} ${outputFilePath}`,function(err,stdout,stderr) {
        if (err) {
            console.error(`Error: ${stderr}`);
            return res.status(500).send('Compression failed.');
        }
        
        res.download(outputFilePath,"compressed.bin",function(){
            fs.unlinkSync(inputFilePath); 
            fs.unlinkSync(outputFilePath); 
        })

    })
})


app.listen(port, function(){
    console.log(`App is listenting on port ${port}`)
})