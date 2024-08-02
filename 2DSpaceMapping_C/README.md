# 2DSpaceMapping autonomous vehicle 🚗 ➡️ HARDWARE SIDE

This is a hardware_branch which contains models for assembly electronic devices/ modules / components. <br />
For more detailed informations about each used modul, visit Wiki 📜 page. <br />
 <br />
## Currently implemented :
  - edge connector
  - MCU PIC24FJ64GA002-I/SP
  - normally opened button TS02-66-50-BK-100-SCR-D
  - WiFi module WRL-17146
  - Slide-Switch SLW-883935-2A-D
  - 2x4 pin socket for WiFi module 5-534206-4
  - MCU socket 1-2199298-9

## Guidance through file hierarchy :
  - Symbols_footprints ➡️ contains .kicad_sym & .kicad_mod files for each used modul / component on the scheme
  - pcb ➡️  contains .png screenshots of .kicad_pcb file
  - schema ➡️  contains .png screenshots of .kicad_sch file and .pdf file exported from KiCad project
  - work ➡️  project work directory. Run ♻️ the pcb_layout_c.kicad_pro form this directory using KiCad7.0 or latest. <br /> (KiCad DOWNLOAD link 🆓⬇️ : https://www.kicad.org/download/windows/)
  - components.csv ➡️ list of used components for this project with Mouser Electronics 🐁 link to each of them. <br /> (https://hr.mouser.com/?gad_source=1&gclid=Cj0KCQiAw6yuBhDrARIsACf94RVZDfmztJkKG18F4qrYKF0iiaaEyd-j5r4cXpUXJpGI24s9JXtPty4aAuFKEALw_wcB)

## KiCad Schema : 
![schema](https://github.com/codeandrelax/2DSpaceMapping_C/assets/93439549/00e11fdd-a0b7-4c42-8b14-3522711066e8)
![schema_left](https://github.com/codeandrelax/2DSpaceMapping_C/assets/93439549/0deb4d70-5c87-4875-b34a-4d397c3f7102)
![schema_right](https://github.com/codeandrelax/2DSpaceMapping_C/assets/93439549/4b538655-138f-4aef-9f1a-e5aa8712c403)

## KiCad PCB Layout: 
![pcb](https://github.com/codeandrelax/2DSpaceMapping_C/assets/93439549/eeb37cc5-fcc2-4e45-8c88-24110b1537c6)

## KiCad 3D View PCB :
![pcb3DView](https://github.com/codeandrelax/2DSpaceMapping_C/assets/93439549/d5eb5ec8-2347-498e-a60c-c8bc70bcaa2a)
![pcb3DView2](https://github.com/codeandrelax/2DSpaceMapping_C/assets/93439549/098549f3-3b63-4a49-ba1d-3b6bb78fdfd7)

## Guidance through Git Bash commands :
1. 		git checkout hardware_branch
2. 		git pull origin hardware_branch
3. 🚧 ...modification... 🚧
4. 		git add .
5. 		git commit -m "Issue#n
   		- Line1
   		- Line2
   		- ..."
     n  - the number of Issue being worked on
7. 		git push origin hardware_branch
