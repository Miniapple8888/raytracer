mat1 = gr.material({0.7, 0.6, 1.0}, {0.5, 0.4, 0.8}, 25)

scene_root = gr.node('root')

ground = gr.nh_sphere('ground', {0, -1200, -500}, 1000)
scene_root:add_child(ground)
ground:set_material(mat1)

parent = gr.node('parent')


-- cube = gr.mesh( 'cube', 'cube.obj' )
-- cube:set_material(mat1)
-- cube:scale(50,50,50)
-- cube:rotate('X', 45)
-- cube:rotate('Y', 45)
-- cube:rotate('Z', 45)
-- cube:translate(-100,-10,100)
-- parent:add_child(cube)

d = gr.mesh( 'dodeca', 'dodeca.obj' )
d:set_material(mat1)
d:scale(50,50,50)
d:rotate('X', 45)
d:rotate('Y', 45)
d:rotate('Z', 45)
d:translate(-100,-10,100)
parent:add_child(d)

-- buckyball = gr.mesh( 'buckyball', 'buckyball.obj' )
-- buckyball:set_material(mat1)
-- buckyball:scale(50,50,50)
-- buckyball:rotate('X', 45)
-- buckyball:rotate('Y', 45)
-- buckyball:rotate('Z', 45)
-- buckyball:translate(-10,-10,100)
-- parent:add_child(buckyball)

-- icosa = gr.mesh( 'icosa', 'icosa.obj' )
-- icosa:set_material(mat1)
-- icosa:scale(50,50,50)
-- icosa:rotate('X', 45)
-- icosa:rotate('Y', 45)
-- icosa:rotate('Z', 45)
-- icosa:translate(-10,-10,100)
-- parent:add_child(icosa)

head = gr.nh_sphere('head', {0, 0, 0}, 50)
-- head = gr.sphere('head')
head:set_material(mat1)
-- head:scale(200,200,200)
head:rotate('X', 45)
head:rotate('Y', 45)
head:rotate('Z', 45)
head:translate(100,200,200)
parent:add_child(head)

--cylinder = gr.mesh( 'cylinder', 'cylinder.obj' )
--cylinder:set_material(mat1)
--cylinder:scale(50,100,50)
--cylinder:rotate('X', 45)
--cylinder:rotate('Y', 45)
--cylinder:rotate('Z', 45)
--cylinder:translate(-10,-10,100)
--parent:add_child(cylinder)

s3 = gr.nh_sphere('s3', {0, 0, 0}, 175)
s3:set_material(mat1)
s3:translate(-100,50,20)
parent:add_child(s3)

s2 = gr.nh_sphere('s2', {0, 0, 0}, 50)
s2:set_material(mat1)
s2:translate(100,-10,0)
parent:add_child(s2)


body = gr.nh_sphere('body', {0, 0, 0}, 50)
body:set_material(mat1)
body:translate(100,-100,0)
parent:add_child(body)

-- cow = gr.mesh( 'cow', 'cow.obj' )
-- cow:set_material(mat1)
-- cow:scale(50,50,50)
-- cow:rotate('X', 45)
-- cow:rotate('Y', 45)
-- cow:rotate('Z', 45)
-- cow:translate(-100,-10,100)
-- parent:add_child(cow)

-- cube = gr.mesh( 'cube', 'cube.obj' )
-- cube:set_material(mat1)
-- cube:scale(50,50,50)
-- cube:rotate('X', 45)
-- cube:rotate('Y', 45)
-- cube:rotate('Z', 45)
-- cube:translate(-100,-10,100)
-- cow:add_child(cube)

-- s5 = gr.nh_sphere('s5', {0, 0, 0}, 100)
-- s5:set_material(mat1)
-- cow:add_child(s5)

-- mickey = gr.mesh('mickey', 'mickey.obj')
-- mickey:scale(200,200,200)
-- mickey:rotate('Z', 90)
-- mickey:rotate('X', -90)
-- mickey:rotate('Y', 180)
-- mickey:set_material(mat1)
-- cow:add_child(mickey)

-- c = gr.nh_box('c', {0,0,0},50)
-- -- c:scale(100,100,100)
-- c:set_material(mat1)
-- c:rotate('X', 45)
-- c:rotate('Y', 45)
-- c:rotate('Z', 45)
-- c:scale(100,100,100)
-- c:translate(100,100,0)
-- parent:add_child(c)

scene_root:add_child(parent)


white_light = gr.light({-100.0, 150.0, 400.0}, {0.9, 0.9, 0.9}, {1, 0, 0})
magenta_light = gr.light({400.0, 100.0, 150.0}, {0.7, 0.0, 0.7}, {1, 0, 0})

gr.render(scene_root, 'sample.png', 512, 512,
	  {0, 0, 800}, {0, 0, -1}, {0, 1, 0}, 50,
	  {0.3, 0.3, 0.3}, {white_light, magenta_light})