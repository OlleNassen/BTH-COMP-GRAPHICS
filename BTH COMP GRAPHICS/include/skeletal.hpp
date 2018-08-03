#ifndef SKELETAL_HPP
#define SKELETAL_HPP
#include <array>
#include "vertex_array.hpp"
#include "buffer.hpp"
#include "node.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>
#include "box.hpp"
#include "shader.hpp"
#include <iostream>

class skeletal_node
{
public:
	skeletal_node(scene::box* mesh = nullptr, glm::vec4 color = glm::vec4(1,1,0,1))
	{
		this->mesh = mesh;
		parent = nullptr;
		this->color = color;
	}
	virtual ~skeletal_node()
	{
		for (auto i = 0u; i < children.size(); i++)
			delete children[i];
	}

	glm::mat4 get_world_transform()const { return world_transform; }

	void set_transform(const glm::mat4& matrix) { this->transform = matrix; }
	glm::mat4 get_transform()const { return this->transform; }

	void set_model_scale(const glm::vec3 scale) { this->model_scale = scale; }
	glm::vec3 get_model_scale()const { return this->model_scale; }

	void set_color(const glm::vec4 color) { this->color = color; }
	glm::vec4 get_color()const { return this->color; }

	void set_mesh(scene::box* ptr){mesh = ptr;}
	scene::box* get_mesh()const{return mesh;}

	void add_child(skeletal_node* child)
	{
		child->parent = this;
		children.emplace_back(child);
	}

	virtual void update(float delta_time)
	{
		if (parent)
		{
			world_transform = parent->world_transform * transform;
		}
		else
		{
			world_transform = transform;
		}

		for (auto& child : children)
		{
			child->update(delta_time);
		}
	}

	virtual void draw(const shader& shade)
	{
		if (mesh)
		{
			shade.uniform("model", transform);
			dynamic_cast<scene::box*>(mesh)->draw();
		}
	}

	std::vector <skeletal_node*>::const_iterator  get_child_iterator_start()
	{
		return  children.begin();
	}
	std::vector <skeletal_node*>::const_iterator  get_child_iterator_end()
	{
		return  children.end();
	}

protected:
	skeletal_node* parent;
	scene::box* mesh;
	glm::mat4 world_transform;
	glm::mat4 transform;
	glm::vec3 model_scale;
	glm::vec4 color;
	std::vector<skeletal_node*> children;
};

class cube_robot : public skeletal_node
{
protected:
	scene::box* cube;
	skeletal_node* head;
	skeletal_node* left_arm;
	skeletal_node* right_arm;
public:
	cube_robot()
	{
		create_cube();
		set_mesh(cube);
		//BODY
		skeletal_node* body = new skeletal_node(cube, glm::vec4(1, 0, 0, 1));    //Red!
		body->set_model_scale(glm::vec3(10, 15, 5));
		glm::mat4 tran(1.f);
		tran[3][0] = 0.f;
		tran[3][1] = 35.f;
		tran[3][2] = 0.f;
		body->set_transform(tran);
		add_child(body);

		//HEAD
		head = new skeletal_node(cube, glm::vec4(0, 1, 0, 1));    //Green!
		head->set_model_scale(glm::vec3(5, 5, 5));
		glm::mat4 tran1(1.f);
		tran1[3][0] = 0.f;
		tran1[3][1] = 30.f;
		tran1[3][2] = 0.f;
		head->set_transform(tran1);
		add_child(head);

		//LEFT ARM
		left_arm = new skeletal_node(cube, glm::vec4(0, 0, 1, 1));    //Blue!
		left_arm->set_model_scale(glm::vec3(3, -18, 3));
		glm::mat4 tran2(1.f);
		tran2[3][0] = 12.f;
		tran2[3][1] = 30.f;
		tran2[3][2] = -1.f;
		left_arm->set_transform(tran2);
		add_child(left_arm);

		//RIGHT ARM
		right_arm = new skeletal_node(cube, glm::vec4(0, 0, 1, 1));    //Blue!
		right_arm->set_model_scale(glm::vec3(3, -18, 3));
		glm::mat4 tran3(1.f);
		tran3[3][0] = 12.f;
		tran3[3][1] = 30.f;
		tran3[3][2] = -1.f;
		right_arm->set_transform(tran3);
		add_child(right_arm);

		//LEFT LEG
		skeletal_node* left_leg = new skeletal_node(cube, glm::vec4(0, 1, 1, 1));    //Red!
		left_leg->set_model_scale(glm::vec3(3, 17.5, 3));
		glm::mat4 tran4(1.f);
		tran4[3][0] = -8.f;
		tran4[3][1] = 0.f;
		tran4[3][2] = 0.f;
		left_leg->set_transform(tran4);
		add_child(left_leg);

		//RIGHT LEG
		skeletal_node* right_leg = new skeletal_node(cube, glm::vec4(0, 1, 1, 1));    //Red!
		right_leg->set_model_scale(glm::vec3(3, 17.5, 3));
		glm::mat4 tran5(1.f);
		tran5[3][0] = 8.f;
		tran5[3][1] = 0.f;
		tran5[3][2] = 0.f;
		right_leg->set_transform(tran5);
		add_child(right_leg);

	}
	~cube_robot()
	{

	}
	void create_cube()
	{
		cube = new scene::box;
	}
	virtual void update(float delta_time)override
	{
		std::cout << delta_time << '\n';
		std::cout << transform[3][0] << " " << transform[3][1] << " " <<
			transform[3][2] << '\n';

		transform = transform *
			glm::rotate(transform, delta_time / 10.0f,
				glm::vec3(0.f, 1.f, 0.f));

		head->set_transform(head->get_transform() *
			glm::rotate(head->get_transform(), -delta_time / 10.0f,
				glm::vec3(0.f, 1.f, 0.f)));

		left_arm->set_transform(left_arm->get_transform() *
			glm::rotate(left_arm->get_transform(), -delta_time / 10.0f,
				glm::vec3(1.f, 0.f, 0.f)));

		right_arm->set_transform(right_arm->get_transform() *
			glm::rotate(right_arm->get_transform(), delta_time / 10.0f,
				glm::vec3(1.f, 0.f, 0.f)));

		skeletal_node::update(delta_time);
	}
};

class skeletal
{
public:
	skeletal();
	~skeletal();
	void render_current(const shader& shader,
		const glm::mat4& world_transform);

private:
	cube_robot* robot;

};

#endif
