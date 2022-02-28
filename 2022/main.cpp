#include <bits/stdc++.h>
using namespace std;

using ll = long long;

using vi = vector<int>;
#define pb push_back
#define rsz resize
#define all(x) begin(x), end(x)
#define sz(x) (int)(x).size()

using pi = pair<int, int>;
#define f first
#define s second
#define mp make_pair

#define LSOne(S) (S & (-S))

#define EPS 1e-9

void setIO(string name = "")
{
  ios_base::sync_with_stdio(0);
  cin.tie(0);

  if (sz(name))
  {
    freopen((name + ".in").c_str(), "r", stdin);
    freopen((name + ".out").c_str(), "w", stdout);
  }
}

struct Skill
{
  string name = "";
  int level = 0;
};

struct Contributer
{
  string name = "";
  int numofSkills = 0;
  int dday = 0;
  vector<Skill> skills;
};

struct Project
{
  int start = 0;
  string name = "";
  int numOfDays = 0;
  int score = 0;
  int bestBefore = 0;
  int numOfSkills = 0;
  vector<Skill> skills;
  vector<string> contributers;
};

vector<string> isYnf3(Project project, vector<Contributer> contributors, int day_i)
{
  map<string, bool> yyy;
  vector<string> contrbs;
  vector<bool> skill(project.numOfSkills, false);
  for (int i = 0; i < project.skills.size(); i++)
  {
    int mn_level_skill = INT_MAX;
    for (int j = 0; j < contributors.size(); j++)
    {
      if (yyy[contributors[j].name])
        continue;
      for (int k = 0; k < contributors[j].skills.size(); k++)
      {
        if (project.skills[i].name == contributors[j].skills[k].name && project.skills[i].level <= contributors[j].skills[k].level && contributors[j].dday < day_i)
        {
          if (contributors[j].skills[k].level < mn_level_skill)
          {
            if (mn_level_skill != INT_MAX)
            {
              contrbs.pop_back();
            }
            contrbs.push_back(contributors[j].name);
            mn_level_skill = contributors[j].skills[k].level;
          }
          skill[i] = true;
        }
      }
    }
    if (!contrbs.empty())
      yyy[contrbs[contrbs.size() - 1]] = true;
  }
  bool ans = true;
  for (auto i : skill)
  {
    ans &= i;
  }
  if (ans)
  {
    for (int i = 0; i < contrbs.size(); i++)
    {
      for (int j = 0; j < contributors.size(); j++)
      {
        if (contributors[j].name == contrbs[i])
        {
          contributors[j].dday += project.numOfDays;
        }
      }
    }
  }
  else
  {
    return vector<string>{};
  }
  return contrbs;
}

int main()
{
  setIO("");
  int numOfContributors, numOfProjects;
  cin >> numOfContributors >> numOfProjects;
  vector<Contributer> contributors(numOfContributors);
  for (int i = 0; i < numOfContributors; i++)
  {
    cin >> contributors[i].name;
    cin >> contributors[i].numofSkills;
    for (int j = 0; j < contributors[i].numofSkills; j++)
    {
      string skillName;
      int skillLevel;
      cin >> skillName >> skillLevel;
      Skill skill;
      skill.name = skillName;
      skill.level = skillLevel;
      contributors[i].skills.pb(skill);
    }
  }
  vector<Project> projects(numOfProjects);
  for (int i = 0; i < numOfProjects; i++)
  {
    cin >> projects[i].name;
    cin >> projects[i].numOfDays;
    cin >> projects[i].score;
    cin >> projects[i].bestBefore;
    cin >> projects[i].numOfSkills;
    for (int j = 0; j < projects[i].numOfSkills; j++)
    {
      string skillName;
      int skillLevel;
      cin >> skillName >> skillLevel;
      Skill skill;
      skill.name = skillName;
      skill.level = skillLevel;
      projects[i].skills.pb(skill);
    }
  }
  sort(all(projects), [](Project a, Project b)
       { if(a.score != b.score) return a.score > b.score;
       else {
      if (a.bestBefore - a.numOfDays != b.bestBefore - b.numOfDays)
        return a.bestBefore - a.numOfDays < b.bestBefore - b.numOfDays;
      else
        return a.numOfDays < b.numOfDays;
    } return true; });
  vector<Project> postponedProjects;
  ll sum = 0;
  for (int i = 0; i < numOfProjects; i++)
  {
    sum += projects[i].numOfDays;
  }
  vector<Project> bestProjects;
  vector<Project> ans;
  for (int i = 0; i < sum; i++)
  {
    for (int j = 0; j < bestProjects.size(); j++)
    {
      if (i - bestProjects[j].start == bestProjects[j].numOfDays - 1)
      {
        ans.push_back(bestProjects[j]);
        for (int l = 0; l < bestProjects[j].contributers.size(); l++)
        {
          for (int k = 0; k < contributors.size(); k++)
          {
            if (bestProjects[j].contributers[l] == contributors[k].name)
            {
              for (int n = 0; n < contributors[k].skills.size(); n++)
              {
                if (contributors[k].skills[n].name == bestProjects[j].skills[l].name && contributors[k].skills[n].level <= bestProjects[j].skills[l].level)
                {
                  contributors[k].skills[n].level++;
                }
              }
            }
          }
        }
        bestProjects.erase(bestProjects.begin() + j);
        j--;
      }
    }
    for (int l = 0; l < postponedProjects.size(); l++)
    {
      postponedProjects[l].contributers = isYnf3(postponedProjects[l], contributors, i);
      if (!postponedProjects[l].contributers.empty())
      {
        postponedProjects[l].start = i;
        bestProjects.pb(postponedProjects[l]);
        postponedProjects.erase(postponedProjects.begin() + l);
        l--;
      }
    }
    for (int l = 0; l < projects.size(); l++)
    {
      projects[l].contributers = isYnf3(projects[l], contributors, i);
      if (!projects[l].contributers.empty())
      {
        projects[l].start = i;
        bestProjects.pb(projects[l]);
        projects.erase(projects.begin() + l);
        l--;
      }
      else
      {
        postponedProjects.pb(projects[l]);
        projects.erase(projects.begin() + l);
        l--;
      }
    }
  }
  cout << ans.size() << endl;
  for (int i = 0; i < ans.size(); i++)
  {
    cout << ans[i].name << endl;
    for (int l = 0; l < ans[i].contributers.size(); l++)
    {
      cout << ans[i].contributers[l] << " ";
    }
    cout << endl;
  }
}
