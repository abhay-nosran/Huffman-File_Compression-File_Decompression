import express from "express"
import { dirname } from "path";
import { fileURLToPath } from "url";
import { exec } from "child_process";
import multer from "multer";
import {stdout,stderr } from "process";
import path from "path";
import fs from "fs";
import os from "os";

const __dirname = dirname(fileURLToPath(import.meta.url));
const port = process.env.PORT || 1200;

// Determine platform-specific executable extension
const executableExt = os.platform() === 'win32' ? '.exe' : '';

const uploadPath = path.join(__dirname, '/server/uploads/');
const exePathCompressor = path.join(__dirname, `/server/compressor${executableExt}`);
const exePathDecompressor = path.join(__dirname, `/server/decompressor${executableExt}`);

// Make executables runnable on Linux
if (os.platform() !== 'win32') {
    try {
        fs.chmodSync(exePathCompressor, '755');
        fs.chmodSync(exePathDecompressor, '755');
    } catch (error) {
        console.error('Error setting executable permissions:', error);
    }
}

const upload = multer({ dest: `${uploadPath}` }) ;
const app = express() ;

app.use(express.static('public'))

app.get("/",(req,res)=>{
    res.sendFile(__dirname+"/public/index.html")
})

app.post('/compress', upload.single('txtFile'), (req, res) => {
    
    const inputFilePath = `${req.file.path}.txt` ;
    console.log(inputFilePath)
    fs.rename(req.file.path,inputFilePath,(err) =>{
        if (err) {
            return res.status(500).send('Error saving the file');
          }
    }) ;
    const outputFilePath = `${req.file.path}.bin` ;

    console.log(inputFilePath) ;
    console.log(outputFilePath) ;

    // Use ./ prefix for Linux executables
    const execPath = os.platform() === 'win32' ? exePathCompressor : `${exePathCompressor}`;
    exec(`${execPath} ${inputFilePath} ${outputFilePath}`, function(err, stdout, stderr) {
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

app.post('/decompress', upload.single('binFile'), (req, res) => {
    
    const inputFilePath = `${req.file.path}.bin` ;
    console.log(inputFilePath);
    fs.rename(req.file.path,inputFilePath,(err) =>{
        if (err) {
            return res.status(500).send('Error saving the file');
          }
    }) ;
    const outputFilePath = `${req.file.path}.txt` ;

    // console.log(inputFilePath) ;
    // console.log(outputFilePath) ;

    // Use ./ prefix for Linux executables
    const execPath = os.platform() === 'win32' ? exePathDecompressor : `${exePathDecompressor}`;
    exec(`${execPath} ${inputFilePath} ${outputFilePath}`, function(err, stdout, stderr) {
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

app.listen(port,()=>{
    console.log(`Listening on port ${port}`)
})

