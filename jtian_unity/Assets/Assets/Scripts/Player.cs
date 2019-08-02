using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class Player : MonoBehaviour
{   
    public GameObject thirdPersonPlayer;
    public float moveSpeed = 0.05f;
    public float rotateSpeed = 10.0f;
    public int playerHP = 100;
    public int enemyDamage = 10;
    public Slider playerUI_HP;
    public ParticleSystem playerVFX_Cast;
    public float speed = 5f;
    public GameObject bullet;
    public Transform firePos;

    private Animator m_PlayerAnimator;
    private Rigidbody m_PlayerRigidbody;
    private Quaternion m_PlayerRotation = Quaternion.identity;
    private Vector3 m_PlayerTransDir;
    private bool m_IsAttack;
    private bool m_IsWalking;
    private bool m_IsDie;
    private bool m_IsHit;

    void Start()
    {
        m_PlayerRigidbody = thirdPersonPlayer.GetComponent<Rigidbody>();
        m_PlayerAnimator = thirdPersonPlayer.GetComponent<Animator>();
        playerUI_HP.value = 1.0f;
    }
    void FixedUpdate()
    {
        if (!m_IsAttack && !m_IsDie)
        {
            float playerHorizontal = Input.GetAxis("Horizontal");
            float playerVertical = Input.GetAxis("Vertical");
            bool isHorizontal = !Mathf.Approximately(playerHorizontal, 0);
            bool isVertical = !Mathf.Approximately(playerVertical, 0);
            m_IsWalking = isHorizontal || isVertical;
            m_PlayerAnimator.SetBool("IsWalking", m_IsWalking);

            Move(playerHorizontal, playerVertical);

            Attack();
        }
    }
    // 移动
    private void Move(float h, float v)
    {
        if (m_IsWalking)
        {
            m_PlayerTransDir.Set(h, 0f, v);
            m_PlayerTransDir.Normalize();

            Vector3 GlobalDirectionForward = thirdPersonPlayer.transform.TransformDirection(Vector3.forward);
            Vector3 ForwardDirection = v * GlobalDirectionForward * Time.fixedDeltaTime;
            Vector3 GlobalDirectionRight = thirdPersonPlayer.transform.TransformDirection(Vector3.right);
            Vector3 RightDirection = h * GlobalDirectionRight * Time.fixedDeltaTime;

            Vector3 MainDirection = ForwardDirection + RightDirection;

            Vector3 rotateDir = Vector3.RotateTowards(transform.forward, m_PlayerTransDir, rotateSpeed * Time.fixedDeltaTime, 0);
            m_PlayerRotation = Quaternion.LookRotation(rotateDir);
            m_PlayerRigidbody.MovePosition(m_PlayerRigidbody.position + m_PlayerTransDir * moveSpeed);
            m_PlayerRigidbody.MoveRotation(m_PlayerRotation);
        }
    }
    // 攻击
    private void Attack()
    {
        if (Input.GetKeyDown(KeyCode.J))
        {
            m_PlayerAnimator.Play("maria_slash");
        }
        if (Input.GetKeyDown(KeyCode.K))
        {
            m_PlayerAnimator.Play("maria_cast");
            
            
        }
    }
    private void AttackStart()
    {
        m_IsAttack = true;
    }
    private void AttackEnd()
    {
        m_IsAttack = false;
    }
    private void SlashDistance()
    {
        Collider[] collider = Physics.OverlapSphere(GameObject.FindWithTag("Sword").transform.position, 1.0f);
       
        if (collider.Length <= 0)
        {
            return;
        }
        for(int i = 1; i < collider.Length; i++)
        {
            // Debug.Log(collider[i].gameObject.name);
            bool isEnemy = collider[i].gameObject.CompareTag("EnemyA") || collider[i].gameObject.CompareTag("EnemyB");
            if (isEnemy)
            {
                collider[i].gameObject.SetActive(false);
            }
        }
    }
    private void Cast()
    {
        playerVFX_Cast.Play();
    }
    // 受击
    private void Hit()
    {
        if (!m_IsHit && !m_IsAttack)
        {
            playerHP -= enemyDamage;
            playerUI_HP.value = (float)playerHP / 100;
            if (playerHP <= 0)
            {
                m_PlayerAnimator.Play("maria_die");
                m_IsDie = true;
            }
        }
        return;
    }
    private void OnTriggerEnter(Collider other)
    {
        bool isEnemy = other.gameObject.CompareTag("EnemyA") || other.gameObject.CompareTag("EnemyB");
        if (isEnemy)
        {
            Hit();
            m_IsHit = true;
        }
    }
    private void OnTriggerExit(Collider other)
    {
        m_IsHit = false;
    }
}
