import Blender


## Filename to store exported data
filename="/home/command/bb/object.c"

### NO NEED TO CHANGE ANYTHING BELOW HERE
oldColor="mmkay..."


## Open file
f = open(filename,"w")

print "*******************"

for a in Blender.Object.Get():  
  print a.data.block_type
  if a.data.block_type == "NMesh":
    print a.data.name
    amesh = Blender.NMesh.GetRaw(a.data.name)
    for v in amesh.verts:
      print "face"
      print v.co[0]
      print v.co[1]
      print v.co[2]




f.write("#include <GL/gl.h>\n");
f.write("#include <GL/glu.h>\n");
f.write("void loadModel(GLfloat x, GLfloat y, GLfloat z, GLfloat rot) {\n");
f.write("glTranslatef(x, y, z);\n");
f.write("glRotatef(rot, 0.0f, 1.0f, 0.0f);\n");
f.write("glColor3f(0.5f, 1.0f, 0.5f);\n");

for o in Blender.Object.Get():

  if o.data.block_type == "NMesh":
    nmesh = Blender.NMesh.GetRaw(o.data.name)
    name =  o.data.name
    type = o.data.block_type
      
    f.write("/*** Object: %s ***/\n" % o.data.name);
    if 1:

      for face in nmesh.faces:
        vn=0
        f.write("glBegin(GL_POLYGON);\n")
        for vertex in face.v:
          ## Build glVertex3f
          data = "  glVertex3f("
          data = data + "%s," % (vertex.co[0] + o.loc[1])
          data = data + "%s," % (vertex.co[1] + o.loc[2])
          data = data + "%s);\n" % (vertex.co[2] + o.loc[0])
          ## Build glNormal3f
          #normal = "  glNormal3f("
          #normal = normal + "%s," % vertex.no[0]
          #normal = normal + "%s," % vertex.no[0]
          #normal = normal + "%s);\n" % vertex.no[2]
          ## Build color changing command
          #color = "  setAmbient(%s," % face.col[vn].r
          
          #color = "  glColor3f(%s," % face.col[0].r
          #color = color + "%s," % face.col[0].g
          #color = color + "%s);\n" % face.col[0].b

          for c in face.col:
#           print "r: ", c.r, " g: ", c.g , " b: ", c.b
            color = "  glColor3f(%s," % c.r
            color = color + "%s," % c.g
            color = color + "%s);\n" % c.b

          ## Output
          ## Color value is outputted only if necessary
          #if oldColor != color:
            #print color
          f.write(color)
          ## Normal info
          #f.write(normal)
          #print normal
          ## Vertex info
          f.write(data)
          #print data
          vn=vn+1
          #oldColor = color
        f.write("glEnd();\n")

f.write("}\n")
print "Export complete"

f.close()