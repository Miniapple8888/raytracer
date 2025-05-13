-- mat1 = gr.material({1., 1.0, 1.}, {0.1, 0.1, 0.1}, 25)
mat3 = gr.material('phong',{1., 0.2, 1.}, {0.1, 0.1, 0.1}, 25)
mat5 = gr.material('phong',{0.1, 0.1, 1.}, {0.1, 0.1, 0.1}, 25)
mat1 = gr.material('metal',{1., 0.,1.}, 0.5)
mat2 = gr.material('dielectric',{0.,1.,1.}, 1.33)
mat4 = gr.material('lambertian',{0.5,1.,0.5})
mat7 = gr.material('lambertian',{0.5,1.,0.5})
-- mat6 = gr.material('isotropic',{0.5,1.,0.})
mat6 = gr.material('henyeyGreenstein',{0.0094,0.6107,0.7924})
text1 = gr.texture('Assets/brickwall.jpg')
text2 = gr.texture('Assets/wood.jpg')
rb1 = gr.rigidbody('kinematic', 100) -- will collide but not move with gravity
rb2 = gr.rigidbody('dynamic', 12)
rb3 = gr.rigidbody('circle', 30)
rb4 = gr.rigidbody('kinematic', 30)
rb5 = gr.rigidbody('kinematic', 30)
rb6 = gr.rigidbody('translation', 30)
scene_root = gr.node('root')

-- s1 = gr.nh_box('s1', {0, 0, 0}, 100)
-- scene_root:add_child(s1)
-- s1:set_material(mat1)

-- p = gr.plane('p', {1,0,0}, {0,1,0}, {0,0,1})
-- scene_root:add_child(p)
-- p:set_material(mat1)

n1 = gr.node('n1')
s1 = gr.nh_sphere('s1', {10, -35, 550}, 25)
s1:set_material(mat3)
-- s1:set_texture(text1)
s1:set_rigidbody(rb3) -- circle
n1:add_child(s1)
scene_root:add_child(n1)


n2 = gr.node('n2')
s2 = gr.nh_sphere('s2', {10, -400, 0}, 300)
s2:set_material(mat4)
s2:set_texture(text1) -- has texture
s2:set_rigidbody(rb1)
n2:add_child(s2)
scene_root:add_child(n2)

n4 = gr.node('n4')
s4 = gr.nh_sphere('s4', {10, 0, 37}, 50)
s4:set_material(mat7)
s4:set_rigidbody(rb2)
n4:add_child(s4)
scene_root:add_child(n4)

n3 = gr.node('n3')
s3 = gr.nh_sphere('s3', {-10, 0, -300}, 250)
s3:set_material(mat1)
n3:add_child(s3)
s3:set_rigidbody(rb1)
scene_root:add_child(n3)

n5 = gr.node('n5')
s5 = gr.nh_sphere('s5', {120, 0, 300}, 70)
s5:set_material(mat2)
n5:add_child(s5)
s5:set_rigidbody(rb6)
scene_root:add_child(n5)

-- n4 = gr.node('n4')
-- s4 = gr.plane('p1', {-100.0, -400.0, -100.0}, {100.0, -400.0, -100.0}, {0.0, -400.0, -200.0})
-- s4:set_material(mat4)
-- n4:add_child(s4)
-- scene_root:add_child(n4)

-- n4 = gr.node('n4')
-- s4 = gr.mesh('cube','cube.obj')
-- s4:set_material(mat5)
-- s4:translate(-100,0,200)
-- s4:scale(400,10,400)
-- n4:add_child(s4)
-- scene_root:add_child(n4)

-- n4 = gr.node('n4')
-- s4 = gr.medium('fog', {10, 30, 550}, 25)
-- s4:set_material(mat6)
-- n4:add_child(s4)
-- scene_root:add_child(n4)

white_light = gr.light({0.0, 20.0, 650.0}, {0.9, 0.9, 0.9}, {0.2, 0.3, 0.5})
-- magenta_light = gr.light({0.0, 60.0, 600.0}, {0.7, 0.0, 0.7}, {0.2, 0.2, 0.3})
-- magenta_light = gr.light({0.0, 60.0, 650.0}, {0.7, 3.0, 0.7}, {0.2, 0.2, 0.3})

-- white_light = gr.light({-200.0, 150.0, 400.0}, {0.9, 0.9, 0.9}, {0.2, 0.3, 0.5})
-- magenta_light = gr.light({400.0, 100.0, 300.0}, {0.7, 0.0, 0.7}, {0.2, 0.2, 0.3})

gr.render(scene_root, 'wall.png', 500, 500, 10,
	  {0, 0, 800}, {0, 0, -800}, {0, 1, 0}, 50,
	  {0.1, 0.1, 0.1}, {white_light})