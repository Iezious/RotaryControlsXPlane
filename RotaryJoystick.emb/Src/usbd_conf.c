/**
  ******************************************************************************
  * @file           : usbd_conf.c
  * @version        : v1.0_Cube
  * @brief          : This file implements the board support package for the USB device library
  ******************************************************************************
  *
  * COPYRIGHT(c) 2018 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  * 1. Redistributions of source code must retain the above copyright notice,
  * this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  * this list of conditions and the following disclaimer in the documentation
  * and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of its contributors
  * may be used to endorse or promote products derived from this software
  * without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
*/
/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx.h"
#include "stm32f1xx_hal.h"
#include "usbd_def.h"
#include "usbd_core.h"
#include "usbd_hid.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

Error reading included file Src/usb_vars.tmp
The problematic instruction:
----------
==> include "Src/usb_vars.tmp" [on line 106, column 1 in usbdconf_f1_c.ftl]
----------

Java backtrace for programmers:
----------
freemarker.template.TemplateException: Error reading included file Src/usb_vars.tmp
	at freemarker.core.Include.accept(Include.java:167)
	at freemarker.core.Environment.visit(Environment.java:208)
	at freemarker.core.ConditionalBlock.accept(ConditionalBlock.java:79)
	at freemarker.core.Environment.visit(Environment.java:208)
	at freemarker.core.MixedContent.accept(MixedContent.java:92)
	at freemarker.core.Environment.visit(Environment.java:208)
	at freemarker.core.Environment.process(Environment.java:188)
	at freemarker.template.Template.process(Template.java:237)
	at com.st.microxplorer.codegenerator.CodeEngine.freemarkerDo(CodeEngine.java:269)
	at com.st.microxplorer.codegenerator.CodeEngine.genCode(CodeEngine.java:190)
	at com.st.microxplorer.codegenerator.CodeGenerator.generateOutputCode(CodeGenerator.java:2913)
	at com.st.microxplorer.codegenerator.CodeGenerator.generateSpecificCode(CodeGenerator.java:2721)
	at com.st.microxplorer.codegenerator.CodeGenerator.generateSpecificCodeFile(CodeGenerator.java:1092)
	at com.st.microxplorer.codegenerator.CodeGenerator.generateCodeFiles(CodeGenerator.java:1229)
	at com.st.microxplorer.codegenerator.CodeGenerator.generateCode(CodeGenerator.java:935)
	at com.st.microxplorer.plugins.projectmanager.engine.ProjectBuilder.generateCode(ProjectBuilder.java:1029)
	at com.st.microxplorer.plugins.projectmanager.engine.ProjectBuilder.createCode(ProjectBuilder.java:913)
	at com.st.microxplorer.plugins.projectmanager.engine.ProjectBuilder.createProject(ProjectBuilder.java:444)
	at com.st.microxplorer.plugins.projectmanager.engine.GenerateProjectThread.run(GenerateProjectThread.java:41)
Caused by: java.io.FileNotFoundException: Template Src/usb_vars.tmp not found.
	at freemarker.template.Configuration.getTemplate(Configuration.java:489)
	at freemarker.core.Environment.getTemplateForInclusion(Environment.java:1459)
	at freemarker.core.Include.accept(Include.java:157)
	... 18 more
