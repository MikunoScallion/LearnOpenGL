	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_STENCIL_TEST);
	//当原物体只通过模板测试时，模板值保持为0
	//当原物体被地面遮挡时（也就是模板测试成功而深度测试失败时）模板值会保持为0，单色轮廓物体会被全部渲染出来
	//只有当原物体同时通过模板测试和深度测试时模板值将被替换成ref值1
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	//当原物体被地面遮挡时模板值会被替换为ref值1，单色轮廓物体只渲染不等于1的部分
	//glStencilOp(GL_KEEP, GL_REPLACE, GL_REPLACE);

	//渲染循环：

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); 

	//绘制地面时禁止写入模板值，因为需要进行测试的只有原物体和单色轮廓物体
	glStencilMask(0x00);
	normalShader.use();
	DrawFloor()  
	
	//绘制原物体时GL_ALWAYS确保原物体总是通过模板测试
	glStencilFunc(GL_ALWAYS, 1, 0xFF); 
	glStencilMask(0xFF); 
	DrawTwoContainers();
	
	//单色轮廓物体只有在模板值不等于1的情况下才会被渲染
	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	glStencilMask(0x00); 
	glDisable(GL_DEPTH_TEST);
	shaderSingleColor.use(); 
	DrawTwoScaledUpContainers();
	glStencilMask(0xFF);
	glEnable(GL_DEPTH_TEST); 